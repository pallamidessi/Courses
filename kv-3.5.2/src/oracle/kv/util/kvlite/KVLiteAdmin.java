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

package oracle.kv.util.kvlite;

import oracle.kv.impl.admin.CommandServiceAPI;
import oracle.kv.impl.admin.param.BootstrapParams;
import oracle.kv.impl.param.ParameterMap;
import oracle.kv.impl.security.login.LoginManager;
import oracle.kv.impl.topo.DatacenterId;
import oracle.kv.impl.topo.DatacenterType;
import oracle.kv.impl.topo.StorageNodeId;
import oracle.kv.impl.util.ConfigurableService.ServiceStatus;
import oracle.kv.impl.util.ServiceUtils;

/**
 * See KVLite.
 * This class creates a simple store with an Admin and RepNode.
 */
public class KVLiteAdmin {

    private static final LoginManager NULL_LOGIN_MGR = null;

    private final String kvstore;
    private final BootstrapParams bp;
    private final ParameterMap policyMap;
    private final int numPartitions;

    public KVLiteAdmin(String kvstore,
                       BootstrapParams bp,
                       ParameterMap policyMap,
                       int numPartitions) {
        this.kvstore = kvstore;
        this.bp = bp;
        this.policyMap = policyMap;
        this.numPartitions = numPartitions;
    }

    public void run()
        throws Exception {

        deployStore();
    }

    /**
     * Use the CommandService to configure/deploy a simple store.
     */
    private void deployStore()
        throws Exception {

        String host = bp.getHostname();
        int port = bp.getRegistryPort();

        CommandServiceAPI admin = ServiceUtils.waitForAdmin
            (host, port, NULL_LOGIN_MGR, 5, ServiceStatus.RUNNING);

        admin.configure(kvstore);
        int planId = admin.createDeployDatacenterPlan(
            "Deploy KVLite", "KVLite", 1, DatacenterType.PRIMARY);
        admin.approvePlan(planId);
        admin.executePlan(planId, false);
        admin.awaitPlan(planId, 0, null);
        admin.assertSuccess(planId);

        if (policyMap != null) {
            admin.setPolicies(policyMap);
        }

        planId = admin.createDeploySNPlan
            ("Deploy Storage Node", new DatacenterId(1), host, port, null);
        admin.approvePlan(planId);
        admin.executePlan(planId, false);
        admin.awaitPlan(planId, 0, null);
        admin.assertSuccess(planId);

        planId = admin.createDeployAdminPlan
            ("Deploy Admin Service", new StorageNodeId(1),
             bp.getAdminHttpPort());
        admin.approvePlan(planId);
        admin.executePlan(planId, false);
        admin.awaitPlan(planId, 0, null);
        admin.assertSuccess(planId);

        admin.addStorageNodePool("KVLitePool");
        admin.addStorageNodeToPool("KVLitePool", new StorageNodeId(1));
        admin.createTopology("KVLite", "KVLitePool", numPartitions, false);
        planId = admin.createDeployTopologyPlan("Deploy KVStore", "KVLite");
        admin.approvePlan(planId);
        admin.executePlan(planId, false);
        admin.awaitPlan(planId, 0, null);
        admin.assertSuccess(planId);
    }
}
