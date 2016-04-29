/*-
 *
 *  This file is part of Oracle NoSQL Database
 *  Copyright (C) 2011, 2015 Oracle and/or its affiliates.  All rights reserved.
 *
 *  Oracle NoSQL Database is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU Affero General Public License
 *  as published by the Free Software Foundation, version 3.
 *
 *  Oracle NoSQL Database is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public
 *  License in the LICENSE file along with Oracle NoSQL Database.  If not,
 *  see <http://www.gnu.org/licenses/>.
 *
 *  An active Oracle commercial licensing agreement for this product
 *  supercedes this license.
 *
 *  For more information please contact:
 *
 *  Vice President Legal, Development
 *  Oracle America, Inc.
 *  5OP-10
 *  500 Oracle Parkway
 *  Redwood Shores, CA 94065
 *
 *  or
 *
 *  berkeleydb-info_us@oracle.com
 *
 *  [This line intentionally left blank.]
 *  [This line intentionally left blank.]
 *  [This line intentionally left blank.]
 *  [This line intentionally left blank.]
 *  [This line intentionally left blank.]
 *  [This line intentionally left blank.]
 *  EOF
 *
 */

package oracle.kv.impl.admin.plan;

import static oracle.kv.impl.admin.plan.Planner.CHANGE_ZONE_TYPE_VERSION;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;

import oracle.kv.impl.admin.Admin;
import oracle.kv.impl.admin.AdminServiceParams;
import oracle.kv.impl.admin.CommandResult;
import oracle.kv.impl.admin.IllegalCommandException;
import oracle.kv.impl.admin.param.AdminParams;
import oracle.kv.impl.admin.param.GlobalParams;
import oracle.kv.impl.admin.param.Parameters;
import oracle.kv.impl.admin.param.SecurityParams;
import oracle.kv.impl.admin.plan.task.AddPartitions;
import oracle.kv.impl.admin.plan.task.BroadcastMetadata;
import oracle.kv.impl.admin.plan.task.BroadcastTopo;
import oracle.kv.impl.admin.plan.task.CheckRNMemorySettings;
import oracle.kv.impl.admin.plan.task.DeployNewRN;
import oracle.kv.impl.admin.plan.task.DeployShard;
import oracle.kv.impl.admin.plan.task.MigratePartition;
import oracle.kv.impl.admin.plan.task.NewNthRNParameters;
import oracle.kv.impl.admin.plan.task.NewRepNodeParameters;
import oracle.kv.impl.admin.plan.task.ParallelBundle;
import oracle.kv.impl.admin.plan.task.RelocateRN;
import oracle.kv.impl.admin.plan.task.Task;
import oracle.kv.impl.admin.plan.task.UpdateAdminParams;
import oracle.kv.impl.admin.plan.task.UpdateDatacenter;
import oracle.kv.impl.admin.plan.task.UpdateDatacenter.UpdateDatacenterV2;
import oracle.kv.impl.admin.plan.task.UpdateHelperHost;
import oracle.kv.impl.admin.plan.task.UpdateNthRNHelperHost;
import oracle.kv.impl.admin.plan.task.UpdateRepNodeParams;
import oracle.kv.impl.admin.plan.task.Utils;
import oracle.kv.impl.admin.plan.task.WaitForAdminState;
import oracle.kv.impl.admin.plan.task.WaitForRepNodeState;
import oracle.kv.impl.admin.topo.TopologyCandidate;
import oracle.kv.impl.admin.topo.TopologyDiff;
import oracle.kv.impl.admin.topo.TopologyDiff.RelocatedPartition;
import oracle.kv.impl.admin.topo.TopologyDiff.RelocatedRN;
import oracle.kv.impl.admin.topo.TopologyDiff.ShardChange;
import oracle.kv.impl.fault.CommandFaultException;
import oracle.kv.impl.metadata.Metadata.MetadataType;
import oracle.kv.impl.security.metadata.SecurityMetadata;
import oracle.kv.impl.security.util.SecurityUtils;
import oracle.kv.impl.topo.AdminId;
import oracle.kv.impl.topo.Datacenter;
import oracle.kv.impl.topo.DatacenterId;
import oracle.kv.impl.topo.DatacenterType;
import oracle.kv.impl.topo.RepGroup;
import oracle.kv.impl.topo.RepGroupId;
import oracle.kv.impl.topo.RepNode;
import oracle.kv.impl.topo.RepNodeId;
import oracle.kv.impl.topo.StorageNode;
import oracle.kv.impl.topo.StorageNodeId;
import oracle.kv.impl.topo.Topology;
import oracle.kv.impl.util.ConfigurableService.ServiceStatus;
import oracle.kv.impl.util.server.LoggerUtils;
import oracle.kv.util.ErrorMessage;

/**
 * Populate the target plan with the sequence of tasks and the new param
 * instances required to move from the current to a new target topology.
 */
class TopoTaskGenerator {

    /* This plan will be populated with tasks by the generator. */
    private final DeployTopoPlan plan;
    private final TopologyCandidate candidate;
    private final Logger logger;
    private final TopologyDiff diff;
    private final Set<DatacenterId> offlineZones;

    TopoTaskGenerator(DeployTopoPlan plan,
                      Topology source,
                      TopologyCandidate candidate,
                      AdminServiceParams adminServiceParams,
                      Set<DatacenterId> offlineZones) {
        this.plan = plan;
        this.candidate = candidate;
        logger = LoggerUtils.getLogger(this.getClass(), adminServiceParams);
        diff = new TopologyDiff(source, null, candidate,
                                plan.getAdmin().getCurrentParameters());
        logger.log(Level.FINE, "task generator sees diff of {0}",
                   diff.display(true));
        this.offlineZones = offlineZones;
    }

    /**
     * Compare the source and the candidate topologies, and generate tasks that
     * will make the required changes.
     *
     * Note that the new repGroupIds in the candidate topology cannot be taken
     * verbatim. The topology class maintains a sequence used to create ids for
     * topology components; one cannot specify the id value for a new
     * component. Since we want to provide some independence between the
     * candidate topology and the topology deployment -- we don't want to
     * require that components be created in precisely some order to mimic
     * the candidate -- we refer to the shards by ordinal value. That is,
     * we refer to the first, second, third, etc shard.
     *
     * The task generator puts the new shards into a list, and generates the
     * tasks with a plan shard index that is the order from that list. The
     * DeployTopoPlan will create and maintain a list of newly generated
     * repgroup/shard ids, and generated tasks will use their ordinal value,
     * as indicated by the planShardIdx and to find the true repgroup id
     * to use.
     */
    void generate() {

        makeDatacenterUpdates();

        /*
         * Execute the RN relocations before creating new RNs, so that a given
         * SN does not become temporarily over capacity, or end up housing
         * two RNs in the same mount point.
         */
        makeRelocatedRNTasks();
        makeCreateRNTasks();

        /*
         * Broadcast all of the above topo changes now so any migrations run
         * smoothly
         */
        plan.addTask(new BroadcastTopo(plan));

        /*
         * Load current security metadata, may be null if it has not been
         * initialized.
         */
        SecurityMetadata secMd = getSecurityMetadata();

        /*
         * Update security metadata with Kerberos configuration information if
         * store is secured and enabled Kerberos.
         */
        secMd = updateWithKerberosInfo(secMd);

        /*
         * Since all RNs are expected to be active now, broadcast the security
         * metadata to them, so that the login authentication is able to work.
         */
        plan.addTask(new BroadcastMetadata<SecurityMetadata>(plan, secMd));

        makePartitionTasks();
    }

    /*
     * Adds tasks related to changes in zones.
     */
    private void makeDatacenterUpdates() {
       /*
        * This method only checks for changes in primary and secondary zones.
        * If more zone types are created, this will have to change.
        */
        assert DatacenterType.values().length == 2;

        final Topology target = candidate.getTopology();

        /*
         * Do the primary zones first, then secondary to avoid leaving a store
         * with no primary zone.
         */
        for (Datacenter dc : target.getDatacenterMap().getAll()) {
            if (dc.getDatacenterType().isPrimary()) {
                makeDatacenterUpdates(target, dc);
            }
        }
        for (Datacenter dc : target.getDatacenterMap().getAll()) {
            if (dc.getDatacenterType().isSecondary()) {
                makeDatacenterUpdates(target, dc);
            }
        }
    }

    private void makeDatacenterUpdates(Topology target, Datacenter dc) {
        final DatacenterId dcId = dc.getResourceId();

        final boolean typeChangeSupported =
            plan.getAdmin().checkAdminGroupVersion(CHANGE_ZONE_TYPE_VERSION,
                                                   offlineZones);

        if (diff.typeChanged(dcId) && !typeChangeSupported) {
            throw new IllegalCommandException(
                          "All Admins must be updated to software version " +
                          CHANGE_ZONE_TYPE_VERSION.getNumericVersionString() +
                          " or greater in order to change the zone type",
                          ErrorMessage.NOSQL_5200,
                          CommandResult.NO_CLEANUP_JOBS);
        }

        /*
         * Add a task to check to see if the zone attributes are up to date.
         * Add the task unconditionally, so it's checked at plan execution
         * time.
         */
        if (typeChangeSupported) {
            plan.addTask(new UpdateDatacenterV2(plan, dcId, dc.getRepFactor(),
                                                dc.getDatacenterType()));
        } else {
            plan.addTask(new UpdateDatacenter(plan, dcId, dc.getRepFactor()));
        }

        /* If the type did not change we are done */
        if (!diff.typeChanged(dcId)) {
            return;
        }

        /* Propogate information about the zone type change */
        plan.addTask(new BroadcastTopo(plan));

        /*
         * If the zone type changed then add tasks to change and restart
         * the Admins and RNs in the zone.
         *
         * If the zone is offline, then only update the admin database, and
         * don't attempt to update its Admins or RNs.  Those changes should be
         * done using repair topology after the zone comes back online and
         * those services, and the SNA, are accessible again.
         */
        final boolean zoneIsOffline = offlineZones.contains(dc.getResourceId());

        /*
         * Add change Admin tasks. Must be done serially in case admins are
         * being restarted.
         */
        final Parameters parameters = plan.getAdmin().getCurrentParameters();
        for (AdminParams ap : parameters.getAdminParams()) {

            final StorageNode sn =
                target.getStorageNodeMap().get(ap.getStorageNodeId());

            if (sn == null) {
                final String msg = 
                        "Inconsistency between the parameters and the " +
                        "topology, " + ap.getStorageNodeId() + " was not " +
                        "found in parameters";
                throw new CommandFaultException(
                    msg, new IllegalStateException(msg),
                    ErrorMessage.NOSQL_5400, CommandResult.TOPO_PLAN_REPAIR);
            }

            if (sn.getDatacenterId().equals(dcId)) {
                final AdminId adminId = ap.getAdminId();
                plan.addTask(new UpdateAdminParams(plan, adminId,
                                                   zoneIsOffline));
                /*
                 * If this is an online primary Admin, wait for it to
                 * restart before proceeding
                 */
                if (!zoneIsOffline && dc.getDatacenterType().isPrimary()) {
                    plan.addTask(
                        new WaitForAdminState(plan, ap.getStorageNodeId(),
                                              adminId, ServiceStatus.RUNNING));
                }
            }
        }

        /*
         * Add change RN tasks. We can safely change one RN per
         * shard in parallel. This is to avoid the newly switched over
         * nodes from becoming masters by outnumbering the existing
         * nodes in the quorum.
         */
        final int rf = dc.getRepFactor();
        final ParallelBundle[] updateTasks = new ParallelBundle[rf];
        final ParallelBundle[] waitTasks = new ParallelBundle[rf];
        for (int i = 0; i < rf; i++) {
            updateTasks[i] = new ParallelBundle();
            waitTasks[i] = new ParallelBundle();
        }

        /*
         * Use the current topology when mapping over RNs to update.  If an RN
         * is not deployed yet, then it doesn't need updating
         */
        final Topology currentTopo = plan.getAdmin().getCurrentTopology();
        for (RepGroup rg: currentTopo.getRepGroupMap().getAll()) {
            int i = 0;
            for (RepNode rn: rg.getRepNodes()) {
                final StorageNode sn =
                        rn.getStorageNodeId().getComponent(target);

                if (sn.getDatacenterId().equals(dcId)) {
                    final RepNodeId rnId = rn.getResourceId();
                    updateTasks[i].addTask(new UpdateRepNodeParams(plan, rnId,
                                                                zoneIsOffline));

                    /*
                     * If this is an online primary node, add a task to wait
                     * for it to restart.
                     */
                    if (!zoneIsOffline && dc.getDatacenterType().isPrimary()) {
                        waitTasks[i].addTask(
                                new WaitForRepNodeState(plan, rnId,
                                                        ServiceStatus.RUNNING));
                    }
                    i++;
                }
            }
        }

        for (int i = 0; i < rf; i++) {
            plan.addTask(updateTasks[i]);
            plan.addTask(waitTasks[i]);
        }
    }

    /**
     * Create tasks to execute all the RN creations.
     */
    private void makeCreateRNTasks() {

        Topology target = candidate.getTopology();

        /* These are the brand new shards */
        List<RepGroupId> newShards = diff.getNewShards();
        for (int planShardIdx = 0;
             planShardIdx < newShards.size();
             planShardIdx++) {

            RepGroupId candidateShard = newShards.get(planShardIdx);
            ShardChange change = diff.getShardChange(candidateShard);
            String snSetDescription = change.getSNSetDescription(target);

            /* We wouldn't expect a brand new shard to host old RNs. */
            if (change.getRelocatedRNs().size() > 0) {
                final String msg =
                    "New shard " + candidateShard + " to be deployed on " +
                    snSetDescription + ", should not host existing RNs " +
                    change.getRelocatedRNs();
                throw new CommandFaultException(
                    msg, new IllegalStateException(msg),
                    ErrorMessage.NOSQL_5200, CommandResult.NO_CLEANUP_JOBS);
            }

            /* Make the shard. */
            plan.addTask(new DeployShard(plan,
                                         planShardIdx,
                                         snSetDescription));

            /* Make all the new RNs that will go on this new shard */

            /*
             * Create the first RN in a primary datacenter first, so it can be
             * the self-electing node and can act as the helper for the
             * remaining nodes, including any non-electable ones
             */
            final List<RepNodeId> newRnIds =
                new ArrayList<RepNodeId>(change.getNewRNs());
            for (final Iterator<RepNodeId> i = newRnIds.iterator();
                 i.hasNext(); ) {
                final RepNodeId rnId = i.next();
                final Datacenter dc = target.getDatacenter(rnId);
                if (dc.getDatacenterType().isPrimary()) {
                    i.remove();
                    newRnIds.add(0, rnId);
                    break;
                }
            }

            for (final RepNodeId proposedRNId : newRnIds) {
                RepNode rn = target.get(proposedRNId);
                String newMountPoint = diff.getMountPoint(proposedRNId);
                plan.addTask(new DeployNewRN(plan,
                                             rn.getStorageNodeId(),
                                             planShardIdx,
                                             newMountPoint));
            }

            /*
             * After the RNs have been created and stored in the topology
             * update their helper hosts.
             */
            for (int i = 0; i < change.getNewRNs().size(); i++) {
                plan.addTask(new UpdateNthRNHelperHost(plan, planShardIdx, i));
                plan.addTask(new NewNthRNParameters(plan, planShardIdx, i));
            }
        }

        /* These are the shards that existed before, but have new RNs */
        for (Map.Entry<RepGroupId, ShardChange> change :
             diff.getChangedShards().entrySet()) {

            RepGroupId rgId = change.getKey();
            if (newShards.contains(rgId)) {
                continue;
            }

            /* Make all the new RNs that will go on this new shard */
            for (RepNodeId proposedRNId : change.getValue().getNewRNs()) {
                RepNode rn = target.get(proposedRNId);
                String newMountPoint = diff.getMountPoint(proposedRNId);
                plan.addTask(new DeployNewRN(plan,
                                             rn.getStorageNodeId(),
                                             rgId,
                                             newMountPoint));
            }

            /*
             * After the new RNs have been created and stored in the topology
             * update the helper hosts for all the RNs in the shard, including
             * the ones that existed before.
             */
            for(RepNode member : target.get(rgId).getRepNodes()) {
                RepNodeId rnId = member.getResourceId();
                plan.addTask(new UpdateHelperHost(plan, rnId, rgId));
                plan.addTask(new NewRepNodeParameters(plan, rnId));
            }
        }
    }

    /**
     * Create tasks to move an RN from one SN to another.
     * The relocation requires three actions that must seen atomic:
     *  1. updating kvstore metadata (topology, params(disable bit, helper
     *   hosts for the target RN and all other members of the HA group) and
     *   broadcast it to all members of the shard. This requires an Admin rep
     *   group quorum
     *  2. updating JE HA rep group metadata (groupDB) and share this with all
     *   members of the JE HA group. This requires a shard master and
     *   quorum. Since we are in the business of actively shutting down one of
     *   the members of the shard, this is clearly a moment of vulnerability
     *  3. Moving the environment data to the new SN
     *  4. Delete the old environment from the old SN
     *
     * Once (1) and (2) are done, the change is logically committed. (1) and
     * (2) can fail due to lack of write availability. The RN is unavailable
     * from (1) to the end of (3). There are a number of options that can short
     * these periods of unavailability, but none that can remove it entirely.
     *
     * One option for making the time from 1->3 shorter is to reach into the JE
     * network backup layer that is the foundation of NetworkRestore, in order
     * to reduce the amount of time used by (3) to transfer data to the new SN.
     * Another option that would make the period from 1-> 2 less
     * vulnerable to lack of quorum is to be able to do writes with a less than
     * quorum number of acks, which is a pending JE HA feature.
     *
     * Both options lessen but do not remove the unavailability periods and the
     * possibility that the changes must be reverse. RelocateRN embodies step
     * 1 and 2 and attempt to clean up if either step fails.
     */
    private void makeRelocatedRNTasks() {

        Set<StorageNodeId> sourceSNs = new HashSet<StorageNodeId>();
        for (Map.Entry<RepGroupId, ShardChange> change :
             diff.getChangedShards().entrySet()) {

            for (RelocatedRN reloc : change.getValue().getRelocatedRNs()) {
                RepNodeId rnId = reloc.getRnId();
                StorageNodeId oldSNId = reloc.getOldSNId();
                StorageNodeId newSNId = reloc.getNewSNId();

                /*
                 * Stop the RN, update its params and topo, update the helper
                 * host param for all members of the group, update its HA group
                 * address, redeploy it on the new SN, and delete the RN from
                 * the original SN once the new RN has come up, and is
                 * consistent with the master. Also ask the original SN to
                 * delete the files from the environment.
                 */
                plan.addTask(new RelocateRN(plan, oldSNId, newSNId, rnId,
                                            diff.getMountPoint(rnId)));
                sourceSNs.add(oldSNId);
            }

            /*
             * SNs that were previously over capacity and have lost an RN may
             * now be able to increase the per-RN memory settings. Check
             * all the source SNs.
             */
            for (StorageNodeId snId : sourceSNs) {
                plan.addTask(new CheckRNMemorySettings(plan, snId));
            }
        }
    }

    /**
     * Partition related tasks. For a brand new deployment, add all the
     * partitions. For redistributions, generate one task per migrated
     * partition.
     */
    private void makePartitionTasks() {

        /* Brand new deployment -- only create new partitions, no migrations. */
        if (diff.getNumCreatedPartitions() > 0) {
            List<RepGroupId> newShards = diff.getNewShards();
            List<Integer> partitionCount =
                new ArrayList<Integer>(newShards.size());
            for (int i = 0; i < newShards.size(); i++) {
                ShardChange change = diff.getShardChange(newShards.get(i));
                int newParts = change.getNumNewPartitions();
                partitionCount.add(newParts);
            }

            plan.addTask(new AddPartitions(plan, partitionCount,
                                           diff.getNumCreatedPartitions()));
            return;
        }

        if (diff.getChangedShards().isEmpty()) {
            return;
        }

        /* A redistribution. Run all partition migrations in parallel. */
        final ParallelBundle bundle = new ParallelBundle();
        for (Map.Entry<RepGroupId,ShardChange> entry :
             diff.getChangedShards().entrySet()){

            RepGroupId targetRGId = entry.getKey();
            List<RelocatedPartition> pChanges =
                entry.getValue().getMigrations();
            for(RelocatedPartition pt : pChanges) {
                Task t = new MigratePartition(plan,
                                              pt.getSourceShard(),
                                              targetRGId,
                                              pt.getPartitionId());
                bundle.addTask(t);
            }
        }
        plan.addTask(bundle);
    }

    /**
     * Return current security metadata stored on admin, may be null if security
     * metadata has not been initialized.
     */
    private SecurityMetadata getSecurityMetadata() {
        final Admin admin = plan.getAdmin();
        return admin.getMetadata(SecurityMetadata.class, MetadataType.SECURITY);
    }

    /**
     * Attempt to store Kerberos configuration information in security metadata
     * if current store enabled security and configured Kerberos as user
     * external authentication method.
     *
     * @return security metadata after updated Kerberos info.
     */
    private SecurityMetadata updateWithKerberosInfo(SecurityMetadata md) {
        final AdminServiceParams adminParams = plan.getAdmin().getParams();
        final SecurityParams securityParams = adminParams.getSecurityParams();

        if (!securityParams.isSecure()) {
            return md;
        }
        final GlobalParams globalParams = adminParams.getGlobalParams();
        if (SecurityUtils.hasKerberos(
                globalParams.getUserExternalAuthMethods())) {

            try {
                Utils.storeKerberosInfo(plan, md);
            } catch (Exception e) {
                throw new IllegalStateException(
                    "Unexpected error occur while storing Kerberos " +
                    "principal in metadata: " + e.getMessage(),
                    e);
            }
        }

        return md;
    }
}
