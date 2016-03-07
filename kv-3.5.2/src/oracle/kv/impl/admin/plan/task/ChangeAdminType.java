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

import static oracle.kv.KVVersion.CURRENT_VERSION;

import oracle.kv.impl.admin.plan.AbstractPlan;
import oracle.kv.impl.topo.AdminId;

/** Needed for compatibility with R3.3. */
public class ChangeAdminType extends SingleJobTask {

    static {
        assert CURRENT_VERSION.getMajor() < 5 : "Remove this class in R5";
    }

    private static final long serialVersionUID = 1L;

    private final AbstractPlan plan;
    private final AdminId adminId;

    public ChangeAdminType(AbstractPlan plan, AdminId adminId) {
        super();
        this.plan = plan;
        this.adminId = adminId;
    }

    @Override
    public State doWork() throws Exception {
        return UpdateAdminParams.update(plan, this, adminId);
    }

    @Override
    public boolean continuePastError() {
        return false;
    }

    @Override
    public boolean restartOnInterrupted() {
        return true;
    }
}
