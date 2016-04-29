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

package oracle.kv.impl.admin;

import static oracle.kv.impl.param.ParameterState.ADMIN_TYPE;
import static oracle.kv.impl.param.ParameterState.REPNODE_TYPE;
import static oracle.kv.impl.util.JsonUtils.createObjectNode;
import static oracle.kv.impl.util.JsonUtils.getArray;
import static oracle.kv.impl.util.JsonUtils.getAsText;
import static oracle.kv.impl.util.ObjectUtil.checkNull;

import java.io.IOException;
import java.io.Serializable;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;

import oracle.kv.KVVersion;
import oracle.kv.impl.admin.CommandResult.CommandFails;
import oracle.kv.impl.admin.CommandResult.CommandSucceeds;
import oracle.kv.impl.admin.TopologyCheck.CONFIG_STATUS;
import oracle.kv.impl.admin.TopologyCheck.CreateRNRemedy;
import oracle.kv.impl.admin.TopologyCheck.RNLocationInput;
import oracle.kv.impl.admin.TopologyCheck.Remedy;
import oracle.kv.impl.admin.TopologyCheck.RemoveRNRemedy;
import oracle.kv.impl.admin.TopologyCheck.TOPO_STATUS;
import oracle.kv.impl.admin.TopologyCheck.UpdateAdminParamsRemedy;
import oracle.kv.impl.admin.TopologyCheck.UpdateRNParamsRemedy;
import oracle.kv.impl.admin.TopologyCheckUtils.SNServices;
import oracle.kv.impl.admin.param.Parameters;
import oracle.kv.impl.admin.param.RepNodeParams;
import oracle.kv.impl.admin.topo.Rules;
import oracle.kv.impl.admin.topo.Rules.Results;
import oracle.kv.impl.admin.topo.Validations.RulesProblem;
import oracle.kv.impl.fault.CommandFaultException;
import oracle.kv.impl.param.LoadParameters;
import oracle.kv.impl.param.ParameterMap;
import oracle.kv.impl.param.ParameterState;
import oracle.kv.impl.rep.RepNodeStatus;
import oracle.kv.impl.rep.admin.IllegalRepNodeServiceStateException;
import oracle.kv.impl.rep.admin.RepNodeAdminAPI;
import oracle.kv.impl.rep.admin.RepNodeAdminFaultException;
import oracle.kv.impl.security.login.LoginManager;
import oracle.kv.impl.sna.StorageNodeAgentAPI;
import oracle.kv.impl.sna.StorageNodeStatus;
import oracle.kv.impl.topo.AdminId;
import oracle.kv.impl.topo.Datacenter;
import oracle.kv.impl.topo.RepGroup;
import oracle.kv.impl.topo.RepGroupId;
import oracle.kv.impl.topo.RepNode;
import oracle.kv.impl.topo.RepNodeId;
import oracle.kv.impl.topo.ResourceId;
import oracle.kv.impl.topo.StorageNodeId;
import oracle.kv.impl.topo.Topology;
import oracle.kv.impl.util.ConfigurableService.ServiceStatus;
import oracle.kv.impl.util.VersionUtil;
import oracle.kv.impl.util.registry.RegistryUtils;
import oracle.kv.util.ErrorMessage;
import oracle.kv.util.Ping;
import oracle.kv.util.Ping.AdminStatusFunction;
import oracle.kv.util.Ping.RepNodeStatusFunction;
import oracle.kv.util.PingDisplay;

import org.codehaus.jackson.JsonNode;
import org.codehaus.jackson.node.ArrayNode;
import org.codehaus.jackson.node.ObjectNode;

/**
 * Object encapsulating various verification methods.
 *
 * <p>Here is an annotated example of the output of the 'verify configuration'
 * command in JSON format.  See the {@link Ping} class for information about
 * fields in common with the output of the ping command, and the
 * {@link CommandJsonUtils} class for information about fields that are pertain
 * to results and are derived from the CommandJsonUtils class.
 *
 * <pre>
 * {
 *   // These command result fields are common to all CLI commands
 *   "operation" : "configure",
 *   "return_code" : 5300,
 *   "description" : "Deploy failed: ConnectionIOException ......" ,
 *   "cmd_cleanup_job" : [ "plan repair-topology" ]
 *   // These fields are all the same as for Ping
 *   "topology" : ...,
 *   "shardStatus" : ...,
 *   "adminStatus" : ...,
 *   "zoneStatus" : ...,
 *   "snStatus" : ...,
 *
 *   "storewideLogName" : "localhost:/kvroot/mystore/log/mystore_{0..N}.log",
 *   "violations" : [ {
 *     "resourceId" : "admin2",
 *     "description" : "ping() failed for admin2 : [...]"
 *   }, {
 *     "resourceId" : "rg1-rn2",
 *     "description" : "ping() failed for rg1-rn2 : [...]"
 *   } ],
 *   // Warnings in same format as violations, if any
 *   "warnings" : [ ]
 * }
 * </pre>
 */
public class VerifyConfiguration {

    private static final String eol = System.getProperty("line.separator");
    private static final Comparator<Problem> resourceComparator =
        new Comparator<Problem>() {
            @Override
            public int compare(Problem p1, Problem p2) {
                return p1.getResourceId().toString().compareTo(
                    p2.getResourceId().toString());
            }
        };
    private final Admin admin;
    private final boolean listAll;
    private final boolean showProgress;
    private final boolean json;
    private final Logger logger;

    private final TopologyCheck topoChecker;

    /* Collect output in JSON format. */
    private final ObjectNode jsonTop;

    /*
     * Found violations are stored here. The collection of violations is cleared
     * each time verify is run.
     */
    private final List<Problem> violations;

    /*
     * Issues that are only advisory, and are not true violations.
     */
    private final List<Problem> warnings;

    /*
     * Violations that have suggested remedies that can be carried out by
     * the topology checker.
     */
    private final Remedies remedies;

    private volatile VerifyType verifyType;
    enum VerifyType {
        TOPOLOGY, UPGRADE, PREREQUISITE;
        @Override
        public String toString() {
            return name().toLowerCase();
        }
    }

    /**
     * Construct a verification object.
     *
     * @param admin the admin node
     * @param showProgress if true, log a line before checking each storage
     * node and its resident services.
     * @param listAll if true, list information for all services checked.
     * @param json if true, produce output in JSON format, otherwise in a
     * human-readable format
     * @param logger output is issued via this logger.
     */
    public VerifyConfiguration(Admin admin,
                               boolean showProgress,
                               boolean listAll,
                               boolean json,
                               Logger logger) {

        this.admin = admin;
        this.showProgress = showProgress;
        this.listAll = listAll;
        this.json = json;
        this.logger = logger;
        violations = new ArrayList<Problem>();
        warnings = new ArrayList<Problem>();
        jsonTop = createObjectNode();
        remedies = new Remedies();

        topoChecker = new TopologyCheck(logger,
                                        admin.getCurrentTopology(),
                                        admin.getCurrentParameters());
    }

    /**
     * Check whether the current topology obeys layout rules, and that the
     * store matches the layout, software version, and parameters described by
     * the most current topology and parameters. The verifyTopology() method
     * may be called multiple times in the life of a VerifyConfiguration
     * instance, but should be called serially.
     *
     * @return true if no violations were found.
     */
    public boolean verifyTopology() {
        return verifyTopology(admin.getCurrentTopology(),
                              admin.getLoginManager(),
                              admin.getCurrentParameters(),
                              true);
    }

    /**
     * Checks that the nodes in the store have been upgraded to the target
     * version.
     *
     * @param targetVersion
     * @return true if all nodes are up to date
     */
    public boolean verifyUpgrade(KVVersion targetVersion,
                                 List<StorageNodeId> snIds) {

        admin.getLogger().log(Level.INFO,
                              "Verifying upgrade to target version: {0}",
                              targetVersion.getNumericVersionString());

        clear();
        verifyType = VerifyType.UPGRADE;

        final Topology topology = admin.getCurrentTopology();
        PingDisplay.topologyOverviewToJson(topology, jsonTop);
        storewideLogNameToJson(admin, jsonTop);
        final RegistryUtils registryUtils =
            new RegistryUtils(topology, admin.getLoginManager());

        if (snIds == null) {
            snIds = topology.getSortedStorageNodeIds();
        }
        final ArrayNode jsonSNs =
            listAll ? jsonTop.putArray("snStatus") : null;
        for (StorageNodeId snId : snIds) {
            verifySNUpgrade(snId, targetVersion, registryUtils, topology,
                            jsonSNs);
        }
        problemsToJson();
        VerifyResults vResults = getResults();
        logger.log(Level.INFO, "{0}", vResults.display());
        return vResults.okay();
    }

    private void clear() {
        violations.clear();
        warnings.clear();
        remedies.clear();
        jsonTop.removeAll();
        verifyType = null;
    }

    private static void storewideLogNameToJson(Admin admin,
                                               ObjectNode jsonTop) {
        jsonTop.put("storewideLogName", admin.getStorewideLogName());
    }

    private static String displayStorewideLogName(JsonNode jsonTop) {
        final String logName = getAsText(jsonTop, "storewideLogName");
        if (logName == null) {
            return "";
        }
        return "See " + logName + " for progress messages";
    }

    private void problemsToJson() {
        problemsToJson(violations, "violations");
        problemsToJson(warnings, "warnings");
    }

    private void problemsToJson(List<Problem> problems, String field) {
        Collections.sort(problems, resourceComparator);
        final ArrayNode jsonProblems = jsonTop.putArray(field);
        for (Problem problem : problems) {
            jsonProblems.add(problemToJson(problem));
        }
    }

    private static ObjectNode problemToJson(Problem problem) {
        final ObjectNode on = createObjectNode();
        on.put("resourceId", problem.getResourceId().toString());
        on.put("description", problem.toString());
        return on;
    }

    private static String displayProblem(JsonNode node) {
        return "[" + getAsText(node, "resourceId", "?") +
            "]\t" + getAsText(node, "description", "");
    }

    private void verifySNUpgrade(StorageNodeId snId,
                                 KVVersion targetVersion,
                                 RegistryUtils registryUtils,
                                 Topology topology,
                                 ArrayNode jsonSNs) {
        StorageNodeStatus snStatus = null;

        try {
            snStatus = registryUtils.getStorageNodeAgent(snId).ping();
        } catch (RemoteException re) {
            violations.add(new RMIFailed(snId, re, "ping()", showProgress,
                                         logger));
        } catch (NotBoundException nbe) {
            violations.add(new RMIFailed(snId, nbe, showProgress, logger));
        }

        if (snStatus != null) {
            final KVVersion snVersion = snStatus.getKVVersion();

            if (snVersion.compareTo(targetVersion) < 0) {
                warnings.add(new UpgradeNeeded(snId,
                                               snVersion,
                                               targetVersion,
                                               showProgress, logger));
            } else {
                /* SN is at or above target. Make sure RNs are up-to-date */
                verifyRNUpgrade(snVersion,
                                topology.getHostedRepNodeIds(snId),
                                registryUtils);
            }
        }

        if (listAll) {
            final ObjectNode jsonSN = PingDisplay.storageNodeToJson(
                topology, topology.get(snId), snStatus);
            logger.info("Verify upgrade: " +
                        PingDisplay.displayStorageNode(jsonSN));
            jsonSNs.add(jsonSN);
        }
    }

    private void verifyRNUpgrade(KVVersion snVersion,
                                 Set<RepNodeId> hostedRepNodeIds,
                                 RegistryUtils registryUtils) {

        ServiceStatus rnStatus = null;

        for (RepNodeId rnId : hostedRepNodeIds) {
            try {
                final RepNodeAdminAPI rn = registryUtils.getRepNodeAdmin(rnId);
                rnStatus = rn.ping().getServiceStatus();
                final KVVersion rnVersion = rn.getInfo().getSoftwareVersion();

                if (rnVersion.compareTo(snVersion) != 0) {
                    warnings.add(new UpgradeNeeded(rnId,
                                                   rnVersion,
                                                   snVersion,
                                                   showProgress, logger));
                }
            } catch (RemoteException re) {
                violations.add(new RMIFailed(rnId, re, "ping()", showProgress,
                                             logger));
            } catch (NotBoundException nbe) {
                violations.add(new RMIFailed(rnId, nbe, showProgress, logger));
            } catch (RepNodeAdminFaultException rnafe) {
                if (rnafe.getFaultClassName().equals(
                    IllegalRepNodeServiceStateException.class.getName())) {

                    violations.add(
                        new StatusNotRight(rnId, ServiceStatus.RUNNING,
                                           rnStatus, showProgress, logger));
                } else {
                    throw rnafe;
                }
            }
        }
    }

    /**
     * Checks that the nodes in the store meet the specified prerequisite
     * version in order to be upgraded to the target version.
     *
     * @param targetVersion
     * @param prerequisiteVersion
     * @return true if no violations were found
     */
    public boolean verifyPrerequisite(KVVersion targetVersion,
                                      KVVersion prerequisiteVersion,
                                      List<StorageNodeId> snIds) {

        admin.getLogger().log(Level.INFO,
                              "Checking upgrade to target version: {0}, " +
                              "prerequisite: {1}",
                   new Object[]{targetVersion.getNumericVersionString(),
                                prerequisiteVersion.getNumericVersionString()});

        clear();
        verifyType = VerifyType.PREREQUISITE;
        final Topology topology = admin.getCurrentTopology();
        PingDisplay.topologyOverviewToJson(topology, jsonTop);
        storewideLogNameToJson(admin, jsonTop);
        final RegistryUtils registryUtils =
            new RegistryUtils(topology, admin.getLoginManager());

        if (snIds == null) {
            snIds = topology.getSortedStorageNodeIds();
        }
        final ArrayNode jsonSNs =
            listAll ? jsonTop.putArray("snStatus") : null;
        for (StorageNodeId snId : snIds) {
            verifySNPrerequisite(snId, targetVersion, prerequisiteVersion,
                                 registryUtils, topology, jsonSNs);
        }
        problemsToJson();
        VerifyResults vResults = getResults();
        logger.log(Level.INFO, "{0}", vResults.display());
        return vResults.okay();
    }

    private void verifySNPrerequisite(StorageNodeId snId,
                                      KVVersion targetVersion,
                                      KVVersion prerequisiteVersion,
                                      RegistryUtils registryUtils,
                                      Topology topology,
                                      ArrayNode jsonSNs) {
        StorageNodeStatus snStatus = null;

        try {
            final StorageNodeAgentAPI sna =
                        registryUtils.getStorageNodeAgent(snId);
            snStatus = sna.ping();
        } catch (RemoteException re) {
            violations.add(new RMIFailed(snId, re, "ping()", showProgress,
                                         logger));
        } catch (NotBoundException nbe) {
            violations.add(new RMIFailed(snId, nbe, showProgress, logger));
        }

        if (snStatus != null) {
            final KVVersion snVersion = snStatus.getKVVersion();

            /* Check if the SN is too old (doesn't meet prereq) */
            if (snVersion.compareTo(prerequisiteVersion) < 0) {
                violations.add(new UpgradeNeeded(snId,
                                                 snVersion,
                                                 prerequisiteVersion,
                                                 showProgress, logger));
                /*
                 * Meets prereq, so check if the SN is too new (downgrade across
                 * minor version)
                 */
            } else if (VersionUtil.compareMinorVersion(snVersion,
                                                       targetVersion) > 0) {
                violations.add(new BadDowngrade(snId,
                                                snVersion,
                                                prerequisiteVersion,
                                                showProgress, logger));
            }
        }

        if (listAll) {
            final ObjectNode jsonSN =
                PingDisplay.storageNodeToJson(topology,
                                              topology.get(snId),
                                              snStatus);
            logger.info("Verify prerequisite: " +
                        PingDisplay.displayStorageNode(jsonSN));
            jsonSNs.add(jsonSN);
        }
    }

    /**
     * Non-private entry point for unit tests. Provides a way to supply an
     * intentionally corrupt topology or parameters, to test for error cases.
     */
    synchronized boolean verifyTopology(Topology topology,
                                        LoginManager loginMgr,
                                        Parameters currentParams,
                                        boolean topoIsDeployed) {

        clear();
        verifyType = VerifyType.TOPOLOGY;
        PingDisplay.topologyOverviewToJson(topology, jsonTop);
        logger.info(PingDisplay.displayTopologyOverview(jsonTop));
        storewideLogNameToJson(admin, jsonTop);
        RegistryUtils registryUtils = new RegistryUtils(topology, loginMgr);
        Results results = Rules.validate(topology, currentParams,
                                         topoIsDeployed);
        violations.addAll(results.getViolations());
        warnings.addAll(results.getWarnings());

        /* Add any remedies that were generated during the validate phase */
        for (RulesProblem rp : results.getViolations()) {
            final Remedy remedy = rp.getRemedy(topoChecker);

            if (remedy != null) {
                remedies.add(remedy);
            }
        }
        checkServices(topology, currentParams, registryUtils);
        problemsToJson();

        VerifyResults vResults = getResults();
        logger.log(Level.INFO, "{0}", vResults.display());
        return vResults.okay();
    }

    /** Store ping and getParams results for an RN. */
    private static class RepNodeInfo {
        RepNodeStatus pingStatus;
        RemoteException pingRemoteException;
        NotBoundException pingNotBoundException;
        LoadParameters getParamsResult;
        RemoteException getParamsRemoteException;
    }

    /**
     * For each SN in the store, contact each service, conduct check.
     */
    /* Suppress Eclipse warning for jsonSNs.add call */
    @SuppressWarnings("null")
    private void checkServices(Topology topology,
                               Parameters currentParams,
                               RegistryUtils registryUtils) {

        /* Collect RN ping and getParams results, and master status info */
        final Map<RepNode, RepNodeInfo> rnInfoMap =
            new HashMap<RepNode, RepNodeInfo>();
        final Map<RepGroupId, RepNodeStatus> masterStatusMap =
            new HashMap<RepGroupId, RepNodeStatus>();
        rnPingAndGetParams(
            topology, registryUtils, rnInfoMap, masterStatusMap);

        /* Use rnInfoMap to provide RepNodeStatus values */
        final RepNodeStatusFunction rnfunc = new RepNodeStatusFunction() {
            @Override
            public RepNodeStatus get(RepNode node) {
                final RepNodeInfo info = rnInfoMap.get(node);
                if (info.pingStatus != null) {
                    return info.pingStatus;
                }
                return null;
            }
        };

        PingDisplay.shardOverviewToJson(topology, rnfunc, jsonTop);

        final Map<AdminId, AdminInfo> allAdminInfo =
            collectAdminInfo(currentParams, registryUtils);
        final AdminStatusFunction adminStatusFunc = new AdminStatusFunction() {
            @Override
            public AdminStatus get(AdminId adminId) {
                final AdminInfo adminInfo = allAdminInfo.get(adminId);
                return (adminInfo != null) ? adminInfo.adminStatus : null;
            }
        };
        PingDisplay.adminOverviewToJson(currentParams, adminStatusFunc,
                                        jsonTop);

        final ArrayNode jsonZones = jsonTop.putArray("zoneStatus");
        for (final Datacenter dc : topology.getSortedDatacenters()) {
            jsonZones.add(PingDisplay.zoneOverviewToJson(topology, dc, rnfunc));
        }

        Map<StorageNodeId, SNServices> sortedResources =
            TopologyCheckUtils.groupServicesBySN(topology, currentParams);
        final ArrayNode jsonSNs =
            listAll ? jsonTop.putArray("snStatus") : null;

        /* The check is done in SN order */
        for (SNServices nodeInfo : sortedResources.values()) {

            StorageNodeId snId = nodeInfo.getStorageNodeId();

            /* If show progress is set, log per Storage Node. */
            if (showProgress) {
                String msg = "Verify: == checking storage node " + snId +
                    " ==";
                logger.info(msg);
            }

            /* Check the StorageNodeAgent on this node. */
            final ObjectNode jsonSN = checkStorageNode(
                registryUtils, topology, currentParams, snId, nodeInfo);
            if (listAll) {
                jsonSNs.add(jsonSN);
            }

            /* If the Admin is there, check it. */
            AdminId adminId = nodeInfo.getAdminId();
            if (adminId != null) {
                checkAdmin(currentParams, adminId, allAdminInfo.get(adminId),
                           jsonSN);
            }

            /* Check all RepNodes on this storage node. */
            final ArrayNode jsonRNs =
                listAll ? jsonSN.putArray("rnStatus") : null;
            for (RepNodeId rnId : nodeInfo.getAllRepNodeIds()) {
                checkRepNode(topology, snId, rnId, currentParams,
                             rnInfoMap.get(topology.get(rnId)),
                             masterStatusMap.get(
                                 new RepGroupId(rnId.getGroupId())),
                             jsonRNs);
            }
        }
    }

    /* Suppress Eclipse warning for rna.getParams call */
    @SuppressWarnings("null")
    private static void rnPingAndGetParams(
        Topology topology,
        RegistryUtils registryUtils,
        Map<RepNode, RepNodeInfo> rnInfoMap,
        Map<RepGroupId, RepNodeStatus> masterStatusMap) {

        for (final RepGroup rg : topology.getRepGroupMap().getAll()) {
            for (final RepNode rn : rg.getRepNodes()) {
                final RepNodeId rnId = rn.getResourceId();
                RepNodeAdminAPI rna = null;
                final RepNodeInfo rnInfo = new RepNodeInfo();
                try {
                    rna = registryUtils.getRepNodeAdmin(rnId);
                    rnInfo.pingStatus = rna.ping();
                } catch (RemoteException re) {
                    rnInfo.pingRemoteException = re;
                } catch (NotBoundException e) {
                    rnInfo.pingNotBoundException = e;
                }
                if (rnInfo.pingStatus != null) {
                    if (rnInfo.pingStatus.getReplicationState().isMaster()) {
                        masterStatusMap.put(new RepGroupId(rnId.getGroupId()),
                                            rnInfo.pingStatus);
                    }
                    if (ServiceStatus.RUNNING.equals(
                            rnInfo.pingStatus.getServiceStatus())) {
                        try {
                            rnInfo.getParamsResult = rna.getParams();
                        } catch (RemoteException re) {
                            rnInfo.getParamsRemoteException = re;
                        }
                    }
                }
                rnInfoMap.put(rn, rnInfo);
            }
        }
    }

    /** Store getAdminStatus results for an Admin. */
    private static class AdminInfo {
        CommandServiceAPI cs;
        AdminStatus adminStatus;
        RemoteException remoteException;
        NotBoundException notBoundException;
    }

    private static Map<AdminId, AdminInfo> collectAdminInfo(
        Parameters params, RegistryUtils regUtils) {

        final Map<AdminId, AdminInfo> allAdminInfo =
            new HashMap<AdminId, AdminInfo>();
        for (final AdminId adminId : params.getAdminIds()) {
            final StorageNodeId snId = params.get(adminId).getStorageNodeId();
            final AdminInfo adminInfo = new AdminInfo();
            try {
                final CommandServiceAPI cs = regUtils.getAdmin(snId);
                adminInfo.cs = cs;
                adminInfo.adminStatus = cs.getAdminStatus();
            } catch (RemoteException e) {
                adminInfo.remoteException = e;
            } catch (NotBoundException e) {
                adminInfo.notBoundException = e;
            }
            allAdminInfo.put(adminId, adminInfo);
        }
        return allAdminInfo;
    }

    /**
     * Ping this admin and check its params.
     */
    private void checkAdmin(Parameters params, AdminId aId,
                            AdminInfo adminInfo, ObjectNode jsonSN) {
        StorageNodeId hostSN = params.get(aId).getStorageNodeId();
        boolean pingProblem = false;
        CommandServiceAPI cs = null;
        AdminStatus adminStatus = null;
        ServiceStatus status = ServiceStatus.UNREACHABLE;
        if (adminInfo.adminStatus != null) {
            cs = adminInfo.cs;
            adminStatus = adminInfo.adminStatus;
            status = adminStatus.getServiceStatus();
        } else if (adminInfo.remoteException != null) {
            final RemoteException re = adminInfo.remoteException;
            violations.add(new RMIFailed(aId, re, "ping()",showProgress,
                                         logger));
            pingProblem = true;
        } else {
            final NotBoundException e = adminInfo.notBoundException;
            violations.add(new RMIFailed(aId, e, showProgress, logger));
            pingProblem = true;
        }

        /*
         * Check the JE HA metadata and SN remote config against the AdminDB
         * for this admin.
         */
        Remedy remedy = topoChecker.checkAdminLocation(admin, aId);
        if (remedy.canFix()) {
            remedies.add(remedy);
        }

        if (!pingProblem) {
            if (status.equals(ServiceStatus.RUNNING)) {
                checkAdminParams(cs, params, aId, hostSN);
            } else {
                violations.add(new StatusNotRight(aId, ServiceStatus.RUNNING,
                                                  status, showProgress,
                                                  logger));
            }
        }

        if (listAll) {
            final ObjectNode jsonAdmin =
                PingDisplay.adminToJson(aId, adminStatus);
            logger.info("Verify: " + PingDisplay.displayAdmin(jsonAdmin));
            jsonSN.put("adminStatus", jsonAdmin);
        }
    }

    /**
     * Ping the storage node. If it does not respond, add it to the problem
     * list. If the version doesn't match that of the admin, also add that to
     * the list.
     */
    @SuppressWarnings("null")
    private ObjectNode checkStorageNode(RegistryUtils regUtils,
                                        Topology topology,
                                        Parameters currentParams,
                                        StorageNodeId snId,
                                        SNServices nodeInfo) {

        boolean pingProblem = false;
        StorageNodeStatus snStatus = null;
        StorageNodeAgentAPI sna = null;
        ServiceStatus status = ServiceStatus.UNREACHABLE;
        try {
            sna = regUtils.getStorageNodeAgent(snId);
            snStatus = sna.ping();
            status = snStatus.getServiceStatus();
        } catch (RemoteException re) {
            violations.add(new RMIFailed(snId, re, "ping()", showProgress,
                                         logger));
            pingProblem = true;
        } catch (NotBoundException e) {
            violations.add(new RMIFailed(snId, e, showProgress, logger));
            pingProblem = true;
        }

        ObjectNode jsonSN = null;
        if (listAll) {
            jsonSN = PingDisplay.storageNodeToJson(
                topology, topology.get(snId), snStatus);
            logger.info("Verify: " + PingDisplay.displayStorageNode(jsonSN));
        }

        if (!pingProblem) {
            if (status.equals(ServiceStatus.RUNNING)) {
                checkSNParams(sna, snId, currentParams, nodeInfo);
            } else {
                violations.add(new StatusNotRight(snId, ServiceStatus.RUNNING,
                                                  status, showProgress,
                                                  logger));
            }

            if (!KVVersion.CURRENT_VERSION.equals(snStatus.getKVVersion())) {
                violations.add(new VersionDifference(snId,
                                                   snStatus.getKVVersion(),
                                                   showProgress, logger));
            }
        }

        return jsonSN;
    }

    /**
     * Check ping and getParams results for this repNode.
     */
    private void checkRepNode(Topology topology,
                              StorageNodeId snId,
                              RepNodeId rnId,
                              Parameters currentParams,
                              RepNodeInfo rnInfo,
                              RepNodeStatus masterStatus,
                              ArrayNode jsonRNs) {

        boolean pingProblem = false;
        ServiceStatus status = ServiceStatus.UNREACHABLE;
        RepNodeStatus rnStatus = null;

        boolean isDisabled = currentParams.get(rnId).isDisabled();
        ServiceStatus expected = isDisabled ? ServiceStatus.UNREACHABLE :
            ServiceStatus.RUNNING;

        if (rnInfo.pingStatus != null) {
            rnStatus = rnInfo.pingStatus;
            status = rnStatus.getServiceStatus();
        } else if (rnInfo.pingRemoteException != null) {
            final RemoteException re = rnInfo.pingRemoteException;
            if (!expected.equals(ServiceStatus.UNREACHABLE)) {
                violations.add(new RMIFailed(rnId, re, "ping()", showProgress,
                                             logger));
                pingProblem = true;
            } else {
                /* The RN is configured as being disabled, issue a warning */
                reportStoppedRN(rnId, snId);
            }
        } else {
            final NotBoundException e = rnInfo.pingNotBoundException;
            if (!expected.equals(ServiceStatus.UNREACHABLE)) {
                violations.add(new RMIFailed(rnId, e, showProgress, logger));
                pingProblem = true;
            } else {
                /* The RN is configured as being disabled, issue a warning */
                reportStoppedRN(rnId, snId);
            }
        }

        if (!pingProblem) {
            if (status.equals(expected)) {
                if (status.equals(ServiceStatus.RUNNING)) {
                    checkRNParams(rnId, topology, currentParams, rnInfo);
                }
            } else {
                violations.add(new StatusNotRight(rnId, expected, status,
                                                  showProgress, logger));
            }
        }

        if (listAll) {
            final ObjectNode jsonRN = PingDisplay.repNodeToJson(
                topology.get(rnId), rnStatus, masterStatus, expected);
            logger.info("Verify: " + PingDisplay.displayRepNode(jsonRN));
            jsonRNs.add(jsonRN);
        }
    }

    /**
     * Report a RN that is not up.
     */
    private void reportStoppedRN(RepNodeId rnId, StorageNodeId snId) {
        warnings.add(new ServiceStopped(rnId, snId, showProgress,
                                        logger, true));

        /*
         * If there are no previously detected errors with this RN, and its
         * location is correct, suggest that it should be restarted.
         */
        final boolean previousRemedy = remedies.remedyExists(rnId);

        if (!previousRemedy) {
            remedies.add(
                new CreateRNRemedy(topoChecker,
                                   new RNLocationInput(TOPO_STATUS.HERE,
                                                       CONFIG_STATUS.HERE),
                                   snId, rnId, null /* jeHAInfo */));
        }
    }

    /** Check SN configuration parameters against the admin database. */
    private void checkSNParams(StorageNodeAgentAPI sna,
                               StorageNodeId snId,
                               Parameters currentParams,
                               SNServices nodeInfo) {

        LoadParameters remoteParams;
        try {
            remoteParams = sna.getParams();
        } catch (RemoteException re) {
            violations.add(new RMIFailed(snId, re, "getParams", showProgress,
                                         logger));
            return;
        }

        if (!checkParams(snId, remoteParams, currentParams.get(snId).getMap(),
                         ParamMismatchLocation.CONFIG)) {
            return;
        }

        /* Check the SNAs mount maps if present */
        ParameterMap mountMap = currentParams.get(snId).getMountMap();
        if (mountMap != null) {
            if (!checkParams(snId, remoteParams, mountMap,
                             ParamMismatchLocation.CONFIG)) {
                return;
            }
        } else {
            if (remoteParams.getMap(ParameterState.BOOTSTRAP_MOUNT_POINTS) !=
                null) {
                violations.add(
                    new ParamMismatch(
                        snId,
                        "Parameter collection " +
                        ParameterState.BOOTSTRAP_MOUNT_POINTS + " missing " +
                        ParamMismatchLocation.CONFIG.describe(),
                        showProgress, logger));
                return;
            }

        }

        if (!checkParams(snId, remoteParams,
                         currentParams.getGlobalParams().getMap(),
                         ParamMismatchLocation.CONFIG)) {
            return;
        }

        /*
         * Make sure all services that are supposed to be on this SN, according
         * to the topo and params, the SN's config.xml, and the JE HA group
         * have location info that are all correct.
         * The RNs to check are the union of those that are in the SN's
         * config.xml and those in the topology.
         */
        topoChecker.saveSNRemoteParams(snId, remoteParams);
        Set<RepNodeId> rnsToCheck = topoChecker.getPossibleRNs(snId);
        for (RepNodeId rnId: rnsToCheck) {
            try {
                final Remedy remedy = topoChecker.checkRNLocation(
                    admin, snId, rnId, false /* calledByDeployNewRN */,
                    false /* makeRNEnabled */, null /* oldSNId */,
                    null /* newMountPoint */);

                if (remedy.canFix()) {
                    logger.log(Level.INFO, "{0}", new Object[] {remedy});
                    Problem p;
                    if (remedy instanceof CreateRNRemedy) {
                        p = new ServiceStopped(rnId,
                                               snId,
                                               showProgress,
                                               logger,
                                               false); /* isDisabled */
                    } else {
                        p = new ParamMismatch(rnId,
                                              remedy.problemDescription(),
                                              showProgress, logger);
                    }

                    violations.add(p);
                    remedies.add(remedy);
                } else {

                    /*
                     * If checking the RN location did not provide a fix, but
                     * the RN appears in the current RN parameters, then check
                     * parameters, and let that be what reports problems and
                     * provides fixes.  Otherwise, just report the parameter
                     * mismatch.
                     */
                    final RepNodeParams currentRepNodeParams =
                        currentParams.get(rnId);
                    if (currentRepNodeParams != null) {
                        checkParams(rnId, remoteParams,
                                    currentRepNodeParams.getMap(),
                                    ParamMismatchLocation.CONFIG);
                    } else {
                        violations.add(
                            new ParamMismatch(rnId,
                                              remedy.problemDescription(),
                                              showProgress, logger));
                    }
                }
            } catch (RemoteException e) {
                violations.add
                (new RMIFailed(snId, e, "checkRNLocation", showProgress,
                               logger));
            } catch (NotBoundException e) {
                violations.add
                (new RMIFailed(snId, e, showProgress, logger));
            }
        }

        /*
         * The Admins to check are the union of those that are in the SN's
         * config.xml and those in AdminDB params.
         */
        ParameterMap adminMap =
            remoteParams.getMapByType(ParameterState.ADMIN_TYPE);
        if (adminMap != null) {
            AdminId aid =
                new AdminId(adminMap.getOrZeroInt(ParameterState.AP_ID));
            if (nodeInfo.getAdminId() == null ||
                !aid.equals(nodeInfo.getAdminId())) {
                violations.add(new ParamMismatch
                             (snId, "Storage Node is managing admin " + aid +
                              " but the admin does not know this",
                              showProgress, logger));
            } else {
                checkParams(aid, remoteParams, currentParams.get(aid).getMap(),
                            ParamMismatchLocation.CONFIG);
            }
        } else if (nodeInfo.getAdminId() != null) {
            violations.add(new ParamMismatch
                         (snId, "Storage Node is not managing an Admin but " +
                          "the admin believes it is",
                          showProgress, logger));
        }
    }

    /**
     * See if this repNode's params match those held in the admin db.
     */
    private void checkRNParams(RepNodeId rnId,
                               Topology topology,
                               Parameters currentParams,
                               RepNodeInfo rnInfo) {

        /* Check results of asking the RN for its params */
        LoadParameters remoteParams;
        if (rnInfo.getParamsResult != null) {
            remoteParams = rnInfo.getParamsResult;
        } else {
            final RemoteException re = rnInfo.getParamsRemoteException;
            violations.add(new RMIFailed(rnId, re, "getParams", showProgress,
                                         logger));
            return;
        }

        checkServiceParams(topology.get(rnId).getStorageNodeId(), rnId,
                           remoteParams, currentParams);
    }

    /**
     * See if this Admin replica's params match those held in the admin db
     * of the master Admin.
     */
    private void checkAdminParams(CommandServiceAPI cs,
                                  Parameters currentParams,
                                  AdminId targetAdminId,
                                  StorageNodeId hostSN) {

        final LoadParameters remoteParams;
        if (admin != null && targetAdminId.equals
            (admin.getParams().getAdminParams().getAdminId())) {

            /*
             * This is the local admin instance -- get the in-memory parameters
             * directly from the the local admin rather than RMI to allow unit
             * tests that use the Admin at a lower level than the RMI layer to
             * run correctly.
             */
            remoteParams = admin.getAllParams();
        } else {
            try {
                remoteParams = cs.getParams();
            } catch (RemoteException re) {
                violations.add(new RMIFailed(targetAdminId, re, "getParams",
                                             showProgress, logger));
                return;
            }
        }

        checkServiceParams(hostSN, targetAdminId, remoteParams, currentParams);
    }

    /**
     * Check that the service, SN, and global parameter values for the
     * in-memory parameters obtained from the specified running service match
     * the reference parameters from the admin DB.
     *
     * @param snId the ID of the SN hosting the service
     * @param rId the resource ID of the service
     * @param paramsToCheck service parameters to check
     * @param referenceParams admin DB parameters to check against
     * @return whether the parameters match
     */
    private boolean checkServiceParams(StorageNodeId snId,
                                       ResourceId rId,
                                       LoadParameters paramsToCheck,
                                       Parameters referenceParams) {
        return
            /* Service parameters */
            checkParams(rId, paramsToCheck, referenceParams.getMap(rId),
                        ParamMismatchLocation.MEMORY) &&
            /* Only consider SN parameters related to services */
            checkParams(snId, paramsToCheck,
                        referenceParams.get(snId).getMap(),
                        ParamMismatchLocation.MEMORY,
                        SNPCompareParamsFilter.INSTANCE) &&
            /* Global parameters */
            checkParams(rId, paramsToCheck,
                        referenceParams.getGlobalParams().getMap(),
                        ParamMismatchLocation.MEMORY);
    }

    /**
     * Check if the parameters associated with the specified resource match the
     * ones in the reference parameters from the admin DB, ignoring
     * parameters that should typically be skipped in comparisons.
     *
     * @param rId the ID of the service
     * @param paramsToCheck service parameters to check
     * @param referenceParamMap admin DB parameters to check against
     * @param location where the parameters to check were found
     * @return whether the parameters match
     */
    private boolean checkParams(ResourceId rId,
                                LoadParameters paramsToCheck,
                                ParameterMap referenceParamMap,
                                ParamMismatchLocation location) {
        return checkParams(rId, paramsToCheck, referenceParamMap,
                           location, CompareParamsFilter.INSTANCE);
    }

    /**
     * Check if the parameters associated with the specified resource match the
     * ones in the reference parameters from the admin DB, using the specified
     * filter to determine which parameters to compare.
     *
     * @param rId the ID of the service
     * @param paramsToCheck service parameters to check
     * @param referenceParamMap admin DB parameters to check against
     * @param location where the parameters to check were found
     * @param filter returns the parameters to compare
     * @return whether the parameters match
     */
    private boolean checkParams(ResourceId rId,
                                LoadParameters paramsToCheck,
                                ParameterMap referenceParamMap,
                                ParamMismatchLocation location,
                                CompareParamsFilter filter) {
        switch (compareParams(paramsToCheck, referenceParamMap, filter)) {
        case NO_DIFFS:
            return true;
        case MISSING:
            violations.add(
                new ParamMismatch(rId,
                                  "Parameter collection " +
                                  referenceParamMap.getType() +
                                  " missing " + location.describe(),
                                  showProgress, logger));
            return false;
        default:
            violations.add(new ParamMismatch(rId,
                                             paramsToCheck.getMapByType(
                                                 referenceParamMap.getType()),
                                             referenceParamMap, location,
                                             showProgress, logger));
            if (referenceParamMap.getType().equals(ADMIN_TYPE)) {
                remedies.add(new UpdateAdminParamsRemedy(topoChecker,
                                                         (AdminId) rId));
            } else if (referenceParamMap.getType().equals(REPNODE_TYPE)) {
                remedies.add(new UpdateRNParamsRemedy(topoChecker,
                                                      (RepNodeId) rId));
            }
            return false;
        }
    }

    /** Results of comparing parameters. */
    public enum CompareParamsResult {
        /** No differences */
        NO_DIFFS,
        /** The parameters were missing */
        MISSING,
        /** There were differences, but no restart is required */
        DIFFS,
        /** There were differences that require a restart */
        DIFFS_RESTART;
    }

    /**
     * Compare service, SN, and global parameters from in-memory values
     * obtained from the specified running service with reference parameters
     * from the admin DB.
     *
     * @param snId the ID of the SN hosting the service
     * @param rId the resource ID of the service
     * @param paramsToCheck service parameters to check
     * @param referenceParams admin DB parameters to compare with
     * @return the result of the comparison
     */
    public static CompareParamsResult compareServiceParams(
        StorageNodeId snId,
        ResourceId rId,
        LoadParameters paramsToCheck,
        Parameters referenceParams) {
        return combineCompareParamsResults(
            /* Service parameters */
            compareParams(paramsToCheck, referenceParams.getMap(rId)),
            /* Only consider SN parameters related to services */
            compareParams(paramsToCheck, referenceParams.get(snId).getMap(),
                          SNPCompareParamsFilter.INSTANCE),
            /* Global parameters */
            compareParams(paramsToCheck,
                          referenceParams.getGlobalParams().getMap()));
    }

    /**
     * Compares parameters obtained from a remote service or configuration
     * parameters to reference parameters, ignoring parameters that should
     * typically be skipped in comparisons.  Checks the parameter map that
     * matches the name and type of the reference map.
     *
     * @param paramsToCheck the remote parameters to check
     * @param referenceParamMap the reference map to compare with
     * @return the result of the comparison
     */
    public static CompareParamsResult compareParams(
        LoadParameters paramsToCheck, ParameterMap referenceParamMap) {
        return compareParams(paramsToCheck, referenceParamMap,
                             CompareParamsFilter.INSTANCE);
    }

    /**
     * Compares parameters obtained from a remote service or configuration
     * parameters to reference parameters, using the specified filter to
     * determine which parameters to compare.  Checks the parameter map that
     * matches the name and type of the reference map.
     *
     * @param paramsToCheck the remote parameters to check
     * @param referenceParamMap the reference map to compare with
     * @param filter returns the parameters to compare
     * @return the result of the comparison
     */
    private static CompareParamsResult compareParams(
        LoadParameters paramsToCheck,
        ParameterMap referenceParamMap,
        CompareParamsFilter filter) {

        ParameterMap mapToCheck = paramsToCheck.getMap(
            referenceParamMap.getName(), referenceParamMap.getType());
        if (mapToCheck == null) {
            return CompareParamsResult.MISSING;
        }
        referenceParamMap = filter.filter(referenceParamMap);
        mapToCheck = filter.filter(mapToCheck);
        if (referenceParamMap.equals(mapToCheck)) {
            return CompareParamsResult.NO_DIFFS;
        }
        if (referenceParamMap.hasRestartRequiredDiff(mapToCheck)) {
            return CompareParamsResult.DIFFS_RESTART;
        }
        return CompareParamsResult.DIFFS;
    }

    /**
     * Combines the results of several parameter comparisons into a single
     * result.  If any parameters were missing, returns MISSING.  Otherwise,
     * returns DIFFS_RESTART if any different parameters require a restart,
     * DIFFS if there were only differences that do not require a restart, and
     * NO_DIFFS otherwise.
     */
    public static CompareParamsResult combineCompareParamsResults(
        CompareParamsResult... results) {
        CompareParamsResult combinedResult = CompareParamsResult.NO_DIFFS;
        for (CompareParamsResult result : results) {
            switch (result) {
            case NO_DIFFS:
                continue;
            case MISSING:
                return CompareParamsResult.MISSING;
            case DIFFS:
                if (combinedResult == CompareParamsResult.NO_DIFFS) {
                    combinedResult = CompareParamsResult.DIFFS;
                }
                continue;
            case DIFFS_RESTART:
                combinedResult = CompareParamsResult.DIFFS_RESTART;
                continue;
            }
        }
        return combinedResult;
    }

    /**
     * Filter to return the parts of a parameter map that should be considered
     * by default when performing a comparison.  This class skips parameters
     * specified by {@link ParameterState#skipParams}, which includes the
     * disabled parameter.
     */
     static class CompareParamsFilter {
         static final CompareParamsFilter INSTANCE = new CompareParamsFilter();
         ParameterMap filter(ParameterMap map) {
            return map.filter(ParameterState.skipParams, false);
        }
    }

    /**
     * Include only StorageNodeParams parameters that are used by services for
     * their own configuration.
     */
    static class SNPCompareParamsFilter extends CompareParamsFilter {
        @SuppressWarnings("hiding")
        static final SNPCompareParamsFilter INSTANCE =
            new SNPCompareParamsFilter();
        @Override
        ParameterMap filter(ParameterMap map) {
            return map.filter(ParameterState.serviceParams, true);
        }
    }

    /**
     * Classes to record violations.
     */
    public interface Problem {
        public ResourceId getResourceId();
    }

    /**
     * Report a service that is stopped either because it is disabled or for
     * some other reason.
     */
    public static class ServiceStopped implements Problem, Serializable {
        private static final long serialVersionUID = 1L;
        private final ResourceId rId;
        private final StorageNodeId snId;
        private final boolean isDisabled;

        ServiceStopped(ResourceId rId,
                       StorageNodeId snId,
                       boolean showProgress,
                       Logger logger,
                       boolean isDisabled) {
            this.rId = rId;
            this.snId = snId;
            this.isDisabled = isDisabled;
            recordProgress(showProgress, logger, this);
        }

        @Override
        public ResourceId getResourceId() {
            return rId;
        }

        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder();
            sb.append(rId + " on " + snId);
            if (isDisabled) {
                sb.append(" was previously stopped and");
            }
            sb.append(" is not running. Consider restarting it with ");
            sb.append("'plan start-service'.");
            return sb.toString();
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + ((rId == null) ? 0 : rId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (obj == null) {
                return false;
            }
            if (!(obj instanceof ServiceStopped)) {
                return false;
            }
            ServiceStopped other = (ServiceStopped) obj;
            if (rId == null) {
                if (other.rId != null) {
                    return false;
                }
            } else if (!rId.equals(other.rId)) {
                return false;
            }
            return true;
        }
    }

    public static class StatusNotRight implements Problem, Serializable {
        private static final long serialVersionUID = 1L;
        private final ResourceId rId;
        private final ServiceStatus expected;
        private final ServiceStatus current;

        StatusNotRight(ResourceId rId,
                       ServiceStatus expected,
                       ServiceStatus current,
                       boolean showProgress,
                       Logger logger) {
            this.rId = rId;
            this.expected = expected;
            this.current = current;
            recordProgress(showProgress, logger, this);
        }

        @Override
        public ResourceId getResourceId() {
            return rId;
        }

        public ServiceStatus getExpectedStatus() {
            return expected;
        }

        public ServiceStatus getCurrentStatus() {
            return current;
        }

        @Override
        public String toString() {
            return "Expected status " + expected + " but was " + current;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result
                    + ((current == null) ? 0 : current.hashCode());
            result = prime * result
                    + ((expected == null) ? 0 : expected.hashCode());
            result = prime * result + ((rId == null) ? 0 : rId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (obj == null) {
                return false;
            }
            if (!(obj instanceof StatusNotRight)) {
                return false;
            }
            StatusNotRight other = (StatusNotRight) obj;
            if (current != other.current) {
                return false;
            }
            if (expected != other.expected) {
                return false;
            }
            if (rId == null) {
                if (other.rId != null) {
                    return false;
                }
            } else if (!rId.equals(other.rId)) {
                return false;
            }
            return true;
        }
    }

    public static class RMIFailed implements Problem, Serializable {
        private static final long serialVersionUID = 1L;
        private final ResourceId rId;
        private final String desc;

        RMIFailed(ResourceId rId, RemoteException e, String methodName,
                  boolean showProgress, Logger logger) {
            this.rId = rId;
            desc = methodName + " failed for " + rId + " : " + e.getMessage();
            recordProgress(showProgress, logger, this);
        }

        RMIFailed(ResourceId rId, NotBoundException e, boolean showProgress,
                  Logger logger) {
            this.rId = rId;
            desc = "No RMI service for " + rId + ": service name=" +
                e.getMessage();
            recordProgress(showProgress, logger, this);
        }

        @Override
        public ResourceId getResourceId() {
            return rId;
        }

        @Override
        public String toString() {
            return desc;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + ((desc == null) ? 0 : desc.hashCode());
            result = prime * result + ((rId == null) ? 0 : rId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (obj == null) {
                return false;
            }
            if (!(obj instanceof RMIFailed)) {
                return false;
            }
            RMIFailed other = (RMIFailed) obj;
            if (desc == null) {
                if (other.desc != null) {
                    return false;
                }
            } else if (!desc.equals(other.desc)) {
                return false;
            }
            if (rId == null) {
                if (other.rId != null) {
                    return false;
                }
            } else if (!rId.equals(other.rId)) {
                return false;
            }
            return true;
        }
    }

    /** Describes where a parameter mismatch was detected. */
    enum ParamMismatchLocation {
        CONFIG("from configuration for service"),
        MEMORY("on service");
        private final String description;
        private ParamMismatchLocation(String description) {
            this.description = description;
        }
        public String describe() { return description; }
    }

    public static class ParamMismatch implements Problem, Serializable {
        private static final long serialVersionUID = 1L;
        private final ResourceId rId;
        private final String mismatch;

        private static String getMismatchMessage(
            ResourceId resourceId,
            ParameterMap remoteCopy,
            ParameterMap adminCopy,
            ParamMismatchLocation location) {

            final ParameterMap onAdminButNotRemote =
                remoteCopy.diff(adminCopy, false);
            final ParameterMap onRemoteButNotAdmin =
                adminCopy.diff(remoteCopy, false);
            final StringBuilder sb = new StringBuilder();
            if (onAdminButNotRemote.size() > 0) {
                sb.append("  Parameters in Admin database but not ")
                    .append(location.describe()).append(" ")
                    .append(resourceId)
                    .append(": ")
                    .append(onAdminButNotRemote.showContents(true));
            }
            if (onRemoteButNotAdmin.size() > 0) {
                if (onAdminButNotRemote.size() > 0) {
                    sb.append("\n");
                }
                sb.append("  Parameters ")
                    .append(location.describe()).append(" ")
                    .append(resourceId)
                    .append(" but not in Admin database: ")
                    .append(onRemoteButNotAdmin.showContents(true));
            }
            return sb.toString();
        }

        ParamMismatch(ResourceId rId, ParameterMap remoteCopy,
                      ParameterMap adminCopy, ParamMismatchLocation location,
                      boolean showProgress, Logger logger) {
            this(rId, getMismatchMessage(rId, remoteCopy, adminCopy, location),
                 showProgress, logger);
        }

        ParamMismatch(ResourceId rId, String msg, boolean showProgress,
                      Logger logger) {

            this.rId = rId;
            mismatch = msg;
            recordProgress(showProgress, logger, this);
        }

        @Override
        public ResourceId getResourceId() {
            return rId;
        }

        @Override
        public String toString() {
            return "Mismatch between metadata in admin service and " + rId +
                ":" + eol + mismatch;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result
                    + ((mismatch == null) ? 0 : mismatch.hashCode());
            result = prime * result + ((rId == null) ? 0 : rId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (obj == null) {
                return false;
            }
            if (!(obj instanceof ParamMismatch)) {
                return false;
            }
            ParamMismatch other = (ParamMismatch) obj;
            if (mismatch == null) {
                if (other.mismatch != null) {
                    return false;
                }
            } else if (!mismatch.equals(other.mismatch)) {
                return false;
            }
            if (rId == null) {
                if (other.rId != null) {
                    return false;
                }
            } else if (!rId.equals(other.rId)) {
                return false;
            }
            return true;
        }
    }

    public static class VersionDifference implements Problem, Serializable {
        private static final long serialVersionUID = 1L;
        private final StorageNodeId snId;
        private final String desc;

        VersionDifference(StorageNodeId snId,
                          KVVersion snVersion,
                          boolean showProgress,
                          Logger logger) {
            this.snId = snId;
            desc = "Admin service version is " + KVVersion.CURRENT_VERSION +
                " but storage node version is " + snVersion;
            recordProgress(showProgress, logger, this);
        }

        @Override
        public ResourceId getResourceId() {
            return snId;
        }

        @Override
        public String toString() {
            return desc;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + ((desc == null) ? 0 : desc.hashCode());
            result = prime * result + ((snId == null) ? 0 : snId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (obj == null) {
                return false;
            }
            if (!(obj instanceof VersionDifference)) {
                return false;
            }
            VersionDifference other = (VersionDifference) obj;
            if (desc == null) {
                if (other.desc != null) {
                    return false;
                }
            } else if (!desc.equals(other.desc)) {
                return false;
            }
            if (snId == null) {
                if (other.snId != null) {
                    return false;
                }
            } else if (!snId.equals(other.snId)) {
                return false;
            }
            return true;
        }
    }

    public static class UpgradeNeeded implements Problem, Serializable {
        private static final long serialVersionUID = 1L;
        private final ResourceId rId;
        private final String desc;

        UpgradeNeeded(ResourceId rId,
                      KVVersion rVersion,
                      KVVersion targetVersion,
                      boolean showProgress,
                      Logger logger) {
            this.rId = rId;
            desc = "Node needs to be upgraded from " +
                   rVersion.getNumericVersionString() +
                   " to version " +
                   targetVersion.getNumericVersionString() +
                   " or newer";

            recordProgress(showProgress, logger, this);
        }

        @Override
        public ResourceId getResourceId() {
            return rId;
        }

        @Override
        public String toString() {
            return desc;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + ((desc == null) ? 0 : desc.hashCode());
            result = prime * result + ((rId == null) ? 0 : rId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (obj == null) {
                return false;
            }
            if (!(obj instanceof UpgradeNeeded)) {
                return false;
            }
            UpgradeNeeded other = (UpgradeNeeded) obj;
            if (desc == null) {
                if (other.desc != null) {
                    return false;
                }
            } else if (!desc.equals(other.desc)) {
                return false;
            }
            if (rId == null) {
                if (other.rId != null) {
                    return false;
                }
            } else if (!rId.equals(other.rId)) {
                return false;
            }
            return true;
        }
    }

    public static class BadDowngrade implements Problem, Serializable {
        private static final long serialVersionUID = 1L;
        private final ResourceId rId;
        private final String desc;

        BadDowngrade(ResourceId rId,
                    KVVersion rVersion,
                    KVVersion targetVersion,
                    boolean showProgress,
                    Logger logger) {
            this.rId = rId;
            desc = "Node cannot be downgraded to " +
                   targetVersion.getNumericVersionString() +
                   " because it is already at a newer minor version " +
                   rVersion.getNumericVersionString();

            recordProgress(showProgress, logger, this);
        }

        @Override
        public ResourceId getResourceId() {
            return rId;
        }

        @Override
        public String toString() {
            return desc;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + ((desc == null) ? 0 : desc.hashCode());
            result = prime * result + ((rId == null) ? 0 : rId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (obj == null) {
                return false;
            }
            if (!(obj instanceof BadDowngrade)) {
                return false;
            }
            BadDowngrade other = (BadDowngrade) obj;
            if (desc == null) {
                if (other.desc != null) {
                    return false;
                }
            } else if (!desc.equals(other.desc)) {
                return false;
            }
            if (rId == null) {
                if (other.rId != null) {
                    return false;
                }
            } else if (!rId.equals(other.rId)) {
                return false;
            }
            return true;
        }
    }

    private String getOperation() {
        StringBuilder operation = new StringBuilder("verify");
        switch(verifyType) {
        case TOPOLOGY:
            operation.append(" configuration");
            break;
        case UPGRADE:
            operation.append(" upgrade");
            break;
        case PREREQUISITE:
            operation.append(" prerequisite");
            break;
        default:
            break;
        }
        if(json) {
            operation.append(" -json");
        }
        if(!showProgress) {
            operation.append(" -silent");
        }
        return operation.toString();
    }

    /*
     * Generate the verify command results based on following rules:
     * 1. If there isn't any violation, return CommandSucceeds
     * 2. If there are both violation and remedy, return CommandFails which
     * error code is 5400 and cleanup job is TOPO_REPAIR.
     * 3. If there is RMIFailed or StatusNotRight violation, we may need retry
     * the verify command again later. Then we will return CommandFails which
     * error code is 5300 and no cleanup job.
     * 4. Otherwise we will return CommandFails which error code is 5200.
     */
    private CommandResult getCommandResult() {
        final boolean hasRemedy = !remedies.isEmpty();
        if (violations == null || violations.size() == 0) {
            if (hasRemedy) {
                return new CommandFails("variable hadRemedy is true although"
                    + " there are no violations",
                    ErrorMessage.NOSQL_5500, CommandResult.NO_CLEANUP_JOBS);
            }
            return new CommandSucceeds(null /* return value */);
        }
        if (hasRemedy) {
            return new CommandFails("There are violations. Please use "
                    + Arrays.toString(CommandResult.TOPO_REPAIR) +
                    " to recover and try again later",
                ErrorMessage.NOSQL_5400, CommandResult.TOPO_REPAIR);
        }
        for (Problem p : violations) {
            if (p instanceof RMIFailed) {
                return new CommandFails("Connect failed, please try again.",
                                        ErrorMessage.NOSQL_5300,
                                        CommandResult.NO_CLEANUP_JOBS);
            }
            if (p instanceof StatusNotRight) {
                final StatusNotRight stNotRight = (StatusNotRight) p;
                final ServiceStatus current = stNotRight.getCurrentStatus();
                final ServiceStatus expected = stNotRight.getExpectedStatus();

                if (expected == ServiceStatus.RUNNING &&
                    (current == ServiceStatus.STARTING ||
                     current == ServiceStatus.ERROR_RESTARTING)) {
                    return new CommandFails(
                        "Waiting for service status, try later",
                        ErrorMessage.NOSQL_5300,
                        CommandResult.NO_CLEANUP_JOBS);
                }
            }
        }
        return new CommandFails("There are violations.",
                                ErrorMessage.NOSQL_5200,
                                CommandResult.NO_CLEANUP_JOBS);
    }

    private String getOutput() {
        if (json) {
            String operation = getOperation();
            CommandResult result = getCommandResult();
            try {
                CommandJsonUtils.updateNodeWithResult(jsonTop, operation,
                                                      result);
                return CommandJsonUtils.toJsonString(jsonTop);
            } catch(IOException e) {
                throw new CommandFaultException(e.getMessage(),
                                                ErrorMessage.NOSQL_5500,
                                                CommandResult.NO_CLEANUP_JOBS);
            }
        }
        final StringBuilder sb = new StringBuilder();
        sb.append("Verify: starting verification of ")
            .append(PingDisplay.displayTopologyOverview(jsonTop)).append(eol);
        sb.append(displayStorewideLogName(jsonTop)).append(eol);
        if (showProgress) {
            if (verifyType == VerifyType.TOPOLOGY) {
                sb.append("Verify: ")
                    .append(PingDisplay.displayShardOverview(jsonTop))
                    .append(eol);
                sb.append("Verify: ")
                    .append(PingDisplay.displayAdminOverview(jsonTop))
                    .append(eol);
                for (JsonNode jsonZone : getArray(jsonTop, "zoneStatus")) {
                    sb.append("Verify: ")
                        .append(PingDisplay.displayZoneOverview(jsonZone))
                        .append(eol);
                }
            }
            for (JsonNode jsonSN : getArray(jsonTop, "snStatus")) {
                final String snId = getAsText(jsonSN, "resourceId");
                if (verifyType == VerifyType.TOPOLOGY) {
                    sb.append("Verify: == checking storage node ")
                        .append(snId).append(" ==").append(eol);
                }
                showProgressProblems(snId, sb);
                sb.append("Verify")
                    .append((verifyType == VerifyType.TOPOLOGY) ? "" :
                            (" " + verifyType))
                    .append(": ")
                    .append(PingDisplay.displayStorageNode(jsonSN)).append(eol);
                final JsonNode jsonAdmin = jsonSN.get("adminStatus");
                if (jsonAdmin != null) {
                    showProgressProblems(
                        getAsText(jsonAdmin, "resourceId"), sb);
                    sb.append("Verify: ")
                        .append(PingDisplay.displayAdmin(jsonAdmin)).
                        append(eol);
                }
                for (JsonNode jsonRN : getArray(jsonSN, "rnStatus")) {
                    showProgressProblems(
                        getAsText(jsonRN, "resourceId"), sb);
                    sb.append("Verify: ")
                        .append(PingDisplay.displayRepNode(jsonRN)).append(eol);
                }
            }
            sb.append(eol);
        }

        final int numViolations = violations.size();
        final int numWarnings = warnings.size();
        if ((numViolations + numWarnings) == 0) {
            sb.append("Verification complete, no violations.");
            return sb.toString();
        }

        sb.append("Verification complete, ").append(numViolations);
        sb.append((numViolations == 1) ? " violation, " : " violations, ");
        sb.append(numWarnings);
        sb.append((numWarnings == 1) ? " note" : " notes");
        sb.append(" found.").append(eol);

        for (JsonNode jsonProblem : getArray(jsonTop, "violations")) {
            sb.append("Verification violation: ")
                .append(displayProblem(jsonProblem)).append(eol);
        }
        for (JsonNode jsonProblem : getArray(jsonTop, "warnings")) {
            sb.append("Verification note: ")
                .append(displayProblem(jsonProblem)).append(eol);
        }
        return sb.toString();
    }

    private void showProgressProblems(String resourceId, StringBuilder sb) {
        if (showProgress) {
            for (Problem problem : violations) {
                if (resourceId.equals(problem.getResourceId().toString())) {
                    sb.append("Verify:         ")
                        .append(problem.getResourceId()).append(": ")
                        .append(problem).append(eol);
                }
            }
            for (Problem problem : warnings) {
                if (resourceId.equals(problem.getResourceId().toString())) {
                    sb.append("Verify:         ")
                        .append(problem.getResourceId()).append(": ")
                        .append(problem).append(eol);
                }
            }
        }
    }

    private static void recordProgress(boolean showProgress,
                                       Logger logger,
                                       Problem problem) {
        if (showProgress) {
            String msg = "Verify:         " + problem.getResourceId() + ": " +
                          problem;
            logger.info(msg);
        }
    }

    public VerifyResults getResults() {
        return new VerifyResults(getOutput(), violations, warnings);
    }

    public TopologyCheck getTopoChecker() {
        return topoChecker;
    }

    /**
     * Returns the list of remedies that should be applied, in order, to repair
     * the configuration problems that were found.  The master Admin ID is used
     * to perform remedies related to the master admin last so that the most
     * progress can be made before potentially needing to restart the process
     * after transferring the master.
     *
     * @param masterAdminId the master Admin ID
     * @return the list of remedies
     */
    public List<Remedy> getRemedies(AdminId masterAdminId) {
        return remedies.getRemedies(masterAdminId);
    }

    /*
     * TODO: Consider adding a more general facility for ordering remedies
     * if we have to add another ordering criterion.
     */
    /**
     * Organize remedies by type, because CreateRNRemedy instances have to be
     * done first and RemoveRNRemedy ones have to be done last.
     */
    private static class Remedies {
        private final List<Remedy> creates;
        private final List<Remedy> removes;
        private final List<Remedy> other;

        Remedies() {
            this.creates = new ArrayList<Remedy>();
            this.removes = new ArrayList<Remedy>();
            this.other = new ArrayList<Remedy>();
        }

        void clear() {
            creates.clear();
            removes.clear();
            other.clear();
        }

        void add(Remedy remedy) {
            if (remedy instanceof CreateRNRemedy) {
                creates.add(remedy);
            } else if (remedy instanceof RemoveRNRemedy) {
                removes.add(remedy);
            } else {
                other.add(remedy);
            }
        }

        List<Remedy> getRemedies(AdminId masterAdminId) {
            checkNull("masterAdminId", masterAdminId);
            final List<Remedy> r = new ArrayList<Remedy>();
            collectRemedies(r, creates, masterAdminId);
            collectRemedies(r, other, masterAdminId);
            collectRemedies(r, removes, masterAdminId);
            return r;
        }

        private void collectRemedies(List<Remedy> result,
                                     List<Remedy> add,
                                     AdminId masterAdminId) {
            boolean foundMasterAdmin = false;
            for (final Remedy remedy : add) {
                if (masterAdminId.equals(remedy.getResourceId())) {
                    foundMasterAdmin = true;
                } else {
                    result.add(remedy);
                }
            }
            if (foundMasterAdmin) {
                for (final Remedy remedy : add) {
                    if (masterAdminId.equals(remedy.getResourceId())) {
                        result.add(remedy);
                    }
                }
            }
        }

        boolean remedyExists(ResourceId resourceId) {
            for (Remedy r : creates) {
                if (r.getResourceId().equals(resourceId)) {
                    return true;
                }
            }

            for (Remedy r : other) {
                if (r.getResourceId().equals(resourceId)) {
                    return true;
                }
            }

            for (Remedy r : removes) {
                if (r.getResourceId().equals(resourceId)) {
                    return true;
                }
            }
            return false;
        }

        boolean isEmpty() {
            return creates.isEmpty() && removes.isEmpty() && other.isEmpty();
        }
    }
}
