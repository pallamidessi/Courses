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

package oracle.kv.impl.admin.topo;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.logging.Logger;

import oracle.kv.impl.admin.Admin;
import oracle.kv.impl.api.TopologyManager;
import oracle.kv.impl.topo.Topology;
import oracle.kv.impl.util.server.LoggerUtils;

import com.sleepycat.je.CursorConfig;
import com.sleepycat.je.LockMode;
import com.sleepycat.je.Transaction;
import com.sleepycat.persist.EntityCursor;
import com.sleepycat.persist.EntityStore;
import com.sleepycat.persist.PrimaryIndex;

/**
 * Topologies are stored in the AdminDB in two forms:
 *
 * 1)a collection of TopologyCandidates, which are named topologies created by
 *   the user.These are in a PrimaryIndex<String, TopologyCandidate>, where
 *   the key is the candidate name.
 *
 * 2)a bounded collection of historical RealizedTopology(s), which keeps
 *   track of all topologies that have deployed by administrative commands. This
 *   is in a PrimaryIndex<Long, RealizedTopology>, where the key is a timestamp.
 *   Realized topologies optionally refer to their originating candidate.
 *
 * The indices used are:
 *
 *
 * PrimaryIndex<String, TopologyCandidate>: list of candidates:
 * PrimaryIndex<Long, RealizedTopology>: Realized topologies, ordered by
 *                            timestamp. The current topology is the latest one.
 *
 * The life cycle of a topology is this:
 *
 * - The user creates a named topology candidate using the "topology create
 * <name> ...". The resulting topology may be deployed, or further manipulated
 * with one of the topology commands that modify layouts, such as topology
 * redistribute, topology rebalance, etc.
 *
 * - This candidate is stored in the candidates collection, and can be viewed,
 * listed, or deleted.
 *
 * - When the user has a topology she likes, she can choose to deploy it with
 * the plan deploy-topology <name> command. Deployments are composed of
 * multiple tasks, and the realized topology is saved periodically as the
 * deployment executes. Since plans can be interrupted, canceled, or incur
 * errors, the realized topology may never reach the precise state of the named
 * topology candidate. The current topology of the store is saved
 * as the last record of the historical topology collection.
 *
 * The historical collection is an audit trail of deployed topologies. It is
 * meant as an audit trail/debugging aid. There is one record per invocation of
 * the deploy-topology command, which preserves the final version of the
 * topology as generated by that command. The historical collection is bounded
 * by time, and versions of the topology that are older than the expiry date
 * are pruned. In a fairly stable store, there may be few deploy-topology
 * commands, and a minimum number of records are kept.
 *
 * TODO: implement pruning.
 *
 * The historical collection can be viewed by a user using the XXX TODO TBW
 * commands.
 */
public class TopologyStore {

    /*
     * Keep a reference to the Admin in order to obtain the entity store,
     * rather than referring to the store directly. Since the Admin refreshes
     * its entity store field upon master failover, using the Admin to get the
     * estore, and opening and closing databases for each usage insulates us
     * from stale handles.
     */
    private final Admin admin;

    /**
     * Start time of the last realized topology from the historical collection.
     * Since the start time is the primary key for the RealizedTopology
     * instance, it's important that the timestamp for new realizedTopologies
     * always ascend. If there is some clock skew between nodes in the Admin
     * replication group, there is the possibility that the start millis is <=
     * to the last saved RealizedTopology. Cache the last start time to use wen
     * generating a unique timestamp.
     */
    private long lastStartMillis;

    /**
     * The maximum number of topo changes that are to be retained in realized
     * topologies that are being saved.
     */
    private final int maxTopoChanges;

    private final Logger logger;

    public TopologyStore(Admin admin) {
        this.admin = admin;
        logger = LoggerUtils.getLogger(this.getClass(), admin.getParams());
        maxTopoChanges =
            admin.getParams().getAdminParams().getMaxTopoChanges();
        lastStartMillis = 0;
    }

    /**
     * Refresh the cached last start time stored in topostore for validating
     * new realized topologies.
     */
    public void initCachedStartTime(Transaction txn) {
        RealizedTopology rt = readCurrentRealizedTopology(txn);
        if (rt == null) {
            lastStartMillis = 0;
        } else {
            lastStartMillis = rt.getStartTime();
        }
    }

    /* ---- Candidates  ---- */

    /**
     * Get a handle onto the candidate primary index. We do not cache a
     * reference to the index in the class so that we can transparently handle
     * the case where there is an admin failover. In those cases, the Admin
     * will close the previous entity store handle and will re-open it. Since
     * this is a low frequency access, it's okay to re-open the index each
     * time, and avoid having to update cached index handles when the entity
     * store reference is updated.
     */
    private PrimaryIndex<String,TopologyCandidate> openCandidates() {
        return admin.getEStore().getPrimaryIndex(String.class,
                                                 TopologyCandidate.class);
    }

    /**
     * @return true if this candidate exists.
     */
    public boolean exists(Transaction txn, String candidateName) {

        PrimaryIndex<String,TopologyCandidate> candidates = openCandidates();
        return candidates.contains(txn, candidateName, LockMode.DEFAULT);
    }

    /**
     * Write this candidate to the store.
     */
    public void save(Transaction txn, TopologyCandidate candidate) {
        PrimaryIndex<String,TopologyCandidate> candidates = openCandidates();

        TopologyManager.pruneChanges(candidate.getTopology(),
                                      Integer.MAX_VALUE,
                                      maxTopoChanges);
        candidates.put(txn, candidate);
    }

    /**
     * Fetch the candidate.
     */
    public TopologyCandidate get(Transaction txn, String candidateName) {
        PrimaryIndex<String,TopologyCandidate> candidates = openCandidates();
        return candidates.get(txn, candidateName, LockMode.DEFAULT);
    }

    /**
     * Delete this candidate.
     */
    public void delete(Transaction txn, String name) {
        PrimaryIndex<String,TopologyCandidate> candidates = openCandidates();
        candidates.delete(txn, name);
    }

    /**
     * Returns a list of names of all candidates. If there are no named
     * topologies, an empty list is returned.
     */
    public List<String> getCandidateNames(Transaction txn) {
        PrimaryIndex<String,TopologyCandidate> candidates = openCandidates();
        EntityCursor<String> nameCursor =
            candidates.keys(txn, CursorConfig.READ_COMMITTED);

        try {
            List<String> names = new ArrayList<String>();
            Iterator<String> iter = nameCursor.iterator();
            while(iter.hasNext()) {
                names.add(iter.next());
            }
            return names;
        } finally {
            nameCursor.close();
        }
    }

    /* ---- Current deployed topology  ---- */

    /**
     * Open the primary index for the historical collection of realized
     * topologies.
     */
    private static PrimaryIndex<Long,RealizedTopology>
                                        openHistory(EntityStore estore) {
        return estore.getPrimaryIndex(Long.class, RealizedTopology.class);
    }

    /**
     * Open the primary index for the historical collection of realized
     * topologies.
     */
    private PrimaryIndex<Long,RealizedTopology> openHistory() {
        return admin.getEStore().getPrimaryIndex(Long.class,
                                                 RealizedTopology.class);
    }

    /**
     * Read the last realized topology from the historical collection.
     */
    private RealizedTopology readCurrentRealizedTopology(Transaction txn) {
        PrimaryIndex<Long,RealizedTopology> history = openHistory();
        EntityCursor<RealizedTopology> cursor =
            history.entities(txn, CursorConfig.READ_COMMITTED);
        RealizedTopology lastRT = null;

        try {
            lastRT = cursor.last();
        } finally {
            cursor.close();
        }
        return (lastRT == null) ? null : lastRT;
    }

    /**
     * Read the last realized topology from the historical collection.
     */
    public static List<RealizedTopology> readLastTopos(EntityStore estore,
                                                       int numToRead) {
        PrimaryIndex<Long,RealizedTopology> history = openHistory(estore);
        EntityCursor<RealizedTopology> cursor =
            history.entities(null, CursorConfig.READ_COMMITTED);

        List<RealizedTopology> latestTopos = new ArrayList<RealizedTopology>();
        RealizedTopology lastRT = null;

        try {
            int count = 0;
            lastRT = cursor.last();
            while (lastRT != null) {
                latestTopos.add(lastRT);
                lastRT = cursor.prev();
                if (++count >= numToRead) {
                    break;
                }
            }
        } finally {
            cursor.close();
        }
        return latestTopos;
    }

    /**
     * Read the current topology from the entity store and return a copy of it,
     * as a  new topology instance.
     */
    public Topology readTopology(Transaction txn) {
        RealizedTopology rt = readCurrentRealizedTopology(txn);
        return (rt == null) ? null : rt.getTopology();
    }

    /**
     * Save the specified topology to the historical store.
     */
    public void save(Transaction txn, RealizedTopology dt) {
        PrimaryIndex<Long,RealizedTopology> history = openHistory();

        TopologyManager.pruneChanges(dt.getTopology(),
                                     Integer.MAX_VALUE,
                                     maxTopoChanges);
        history.put(txn, dt);
    }

    /* ---- History of deployed topologies  ---- */

    /**
     * Dump the history.
     * @param concise if false, show the topology itself.
     */
    public List<String> displayHistory(Transaction txn, boolean concise) {
        List<String> display = new ArrayList<String>();

        PrimaryIndex<Long,RealizedTopology> history = openHistory();
        EntityCursor<RealizedTopology> cursor =
            history.entities(txn, CursorConfig.READ_COMMITTED);
        try {
            for (RealizedTopology dt: cursor) {
                display.add(dt.display(concise));
            }
        } finally {
            cursor.close();
        }
        return display;
    }

    /**
     * Since the start time is the primary key for the historical collection,
     * ensure that the start time for any new realized topology is greater than
     * the start time recorded for the current topology. Due to clock skew in
     * the HA rep group, conceivably the start time could fail to advance if
     * there is admin rep node failover.
     */
    public synchronized long validateStartTime(long proposedStartTime) {

        /* The proposed start time is greater, as expected. */
        if (proposedStartTime > lastStartMillis) {
            lastStartMillis = proposedStartTime;
            return proposedStartTime;
        }

        /*
         * For some reason, the proposed start time has receded. Manufacture
         * a new start time. This should be fairly infrequent.
         */
        lastStartMillis++;
        logger.info("TopologyStore: proposedStartTime of " + proposedStartTime +
                    " is less than cached time, so use topoStore lastStartTime"+
                    " of " + lastStartMillis);
        return lastStartMillis;
    }
}