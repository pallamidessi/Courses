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

package oracle.kv.impl.admin.plan.task;

import java.util.logging.Level;

import oracle.kv.impl.admin.plan.AbstractPlan;
import oracle.kv.impl.topo.ResourceId;

import com.sleepycat.je.OperationFailureException;
import com.sleepycat.je.rep.MasterStateException;
import com.sleepycat.je.rep.MemberActiveException;
import com.sleepycat.je.rep.MemberNotFoundException;
import com.sleepycat.je.rep.UnknownMasterException;
import com.sleepycat.je.rep.util.ReplicationGroupAdmin;

/**
 * Base class for classes that update parameters.
 */
abstract class BasicUpdateParams extends SingleJobTask {
    private static final long serialVersionUID = 1L;

    final AbstractPlan plan;

    BasicUpdateParams(AbstractPlan plan) {
        this.plan = plan;
    }

    /**
     * Delete a node from the JE replication group.
     */
    static boolean deleteMember(AbstractPlan plan,
                                ReplicationGroupAdmin jeAdmin,
                                String targetNodeName,
                                ResourceId targetId) {

        plan.getLogger().fine("Deleting member: " + targetNodeName);
        final long timeout = 90000;
        final long check = 1000;
        final long stop = System.currentTimeMillis() + timeout;
        while (true) {
            try {
                jeAdmin.deleteMember(targetNodeName);
                return true;
            } catch (IllegalArgumentException iae) {
                /* Already a secondary, ignore */
                return true;
            } catch (UnknownMasterException ume) {
                if (System.currentTimeMillis() > stop) {
                    logError(plan, targetId,
                             "the master was not found for deleteMember: " +
                             ume);
                    plan.getLogger().log(Level.INFO, "Exception", ume);
                    return false;
                }
                plan.getLogger().info(
                    "Waiting to retry deleteMember after unknown master");
                try {
                    Thread.sleep(check);
                } catch (InterruptedException e) {
                    logError(plan, targetId,
                             "waiting for the master for deleteMember was" +
                             " interrupted");
                    return false;
                }
            } catch (MemberActiveException mae) {
                /* This is unlikely as we just stopped the node */
                logError(plan, targetId,
                         "it is active when calling deleteMember");
                return false;
            } catch (MemberNotFoundException mnfe) {
                /* Already deleted, ignore */
                return true;
            } catch (MasterStateException mse) {
                logError(plan, targetId,
                         "it was the master when calling deleteMember");
                return false;
            } catch (OperationFailureException ofe) {
                logError(plan, targetId, "unexpected exception: " + ofe);
                return false;
            }
        }
    }

    /**
     * Log an error that occurred while updating parameters.
     */
    static void logError(AbstractPlan plan,
                         ResourceId targetId,
                         String cause) {
        plan.getLogger().log(
            Level.INFO, "Couldn''t update parameters for {0} because {1}",
            new Object[] { targetId, cause });
    }
}
