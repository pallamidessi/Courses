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

import oracle.kv.impl.admin.plan.DeployDatacenterPlan;

import com.sleepycat.persist.model.Persistent;

/**
 * A task for creating a Datacenter and registering it with the Topology.
 */
@Persistent(version=1)
public class DeployDatacenter extends SingleJobTask {

    private static final long serialVersionUID = 1L;

    private DeployDatacenterPlan plan;

    public DeployDatacenter(DeployDatacenterPlan plan) {
        super();
        this.plan = plan;
    }

    /*
     * No-arg ctor for use by DPL.
     */
    @SuppressWarnings("unused")
    private DeployDatacenter() {
    }

    /**
     * Failure and interruption statuses are set by the PlanExecutor, to
     * generalize handling or exception cases.
     */
    @Override
    public State doWork()
        throws Exception {

        /* Nothing to do for now. */
        return State.SUCCEEDED;
    }

    @Override
    public String toString() {
        return super.toString() +  " zone=" + plan.getDatacenterName();
    }

    @Override
    public boolean continuePastError() {
        return false;
    }
}
