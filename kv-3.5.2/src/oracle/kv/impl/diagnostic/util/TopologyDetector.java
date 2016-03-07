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

package oracle.kv.impl.diagnostic.util;

import static oracle.kv.impl.util.TopologyLocator.HOST_PORT_SEPARATOR;

import java.io.IOException;
import java.net.URI;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.List;

import oracle.kv.AuthenticationFailureException;
import oracle.kv.LoginCredentials;
import oracle.kv.impl.admin.CommandServiceAPI;
import oracle.kv.impl.admin.param.StorageNodeParams;
import oracle.kv.impl.security.login.LoginManager;
import oracle.kv.impl.security.util.KVStoreLogin;
import oracle.kv.impl.topo.StorageNode;
import oracle.kv.impl.topo.StorageNodeId;
import oracle.kv.impl.topo.Topology;
import oracle.kv.impl.util.TopologyLocator;
import oracle.kv.impl.util.registry.RegistryUtils;

import com.sleepycat.je.rep.ReplicatedEnvironment.State;

/**
 * Detect a current Topology given a list of SN registry locations
 */
public class TopologyDetector {
    /*
     * The different login manager types used to access topology component.
     */
    private enum LoginManagerType {
        ADMINLOGIN, /* admin login manager. */
        REPNODELOGIN; /* repnode login manager */
    }

    private String host = null;
    private int port = -1;
    private String user = null;
    private String securityFile = null;

    /* Force to logon RepNode to get topology */
    private boolean forceRepNodeLogon = false;

    private Topology topology = null;
    private CommandServiceAPI commandService = null;

    public TopologyDetector(String host,
            int port,
            String user,
            String securityFile) {
        this.host = host;
        this.port = port;
        this.user = user;
        this.securityFile = securityFile;
    }

    /**
     * Only for unit test
     *
     * @param host
     * @param port
     * @param user
     * @param securityFile
     */
    public TopologyDetector(String host,
            int port,
            String user,
            String securityFile,
            boolean forceRepNodeLogon) {
        this(host, port, user, securityFile);
        this.forceRepNodeLogon = forceRepNodeLogon;
    }

    /**
     * Get topology by host/port of storage node
     *
     * @return Topology
     * @throws Exception
     */
    public Topology getTopology() throws Exception {
        LoginManager loginMgr;
        try {
            if (!forceRepNodeLogon) {
                /* Get topology from admin node */
                loginMgr = getLoginManager(LoginManagerType.ADMINLOGIN);
                commandService = getAdmin(host, port, loginMgr);
                topology = commandService.getTopology();
                return topology;
            }
        } catch (Exception e) {
            commandService = null;
        }
        /*
         * Get topology from repnode admin when cannot get topology from admin
         * node
         */
        final String regHostPort = host + HOST_PORT_SEPARATOR + port;

        try {
            /* Get repnode login manager */
            loginMgr = getLoginManager(LoginManagerType.REPNODELOGIN);
            topology = TopologyLocator.get(new String[] { regHostPort }, 0,
                                           loginMgr,
                                           null/* Expected store name */);

            /* Get CommandService from topology */
            List<StorageNode> snList = topology.getSortedStorageNodes();
            for (StorageNode sn : snList) {
                commandService = getAdmin(sn.getHostname(),
                                          sn.getRegistryPort(), loginMgr);

                if (commandService != null) {
                    break;
                }
            }

            return topology;
        } catch (Exception ex) {
            /*
             * Output message when security mismatch between the client and
             * server
             */
            if (ex.getCause() != null &&
                ex.getCause().getMessage() != null &&
                ex.getCause().getMessage().contains("which may be " +
                        "caused by a security mismatch between the " +
                        "client and server")) {
                throw new Exception("Security mismatch between the client" +
                        " server");
            }

            throw new Exception("Could not establish an initial Topology" +
                               " from: " + regHostPort + ":" + ex.getMessage());
        }
    }

    /**
     * Get an admin from topology
     *
     * @param hostname
     * @param portNum
     * @param loginMgr
     * @return
     * @throws NotBoundException
     * @throws RemoteException
     */
    private CommandServiceAPI getAdmin(String hostname, int portNum,
                                       LoginManager loginMgr)
            throws Exception {
        CommandServiceAPI cs = RegistryUtils.getAdmin(hostname, portNum,
                                                      loginMgr);
        State adminState = cs.getAdminStatus().getReplicationState();
        if (adminState != null) {
            switch (adminState) {
                case MASTER:
                    break;
                case REPLICA:
                    URI rmiaddr = cs.getMasterRmiAddress();
                    String adminHostname = rmiaddr.getHost();
                    int adminRegistryPort = rmiaddr.getPort();
                    cs = getAdmin(adminHostname, adminRegistryPort, loginMgr);
                    break;
                case DETACHED:
                case UNKNOWN:
                    cs = null;
                    break;
            }
            return cs;
        }
        return null;
    }

    public String getRootDirPath(StorageNode storageNode) {
        /*
         * Get root directory from command service when commandService is not
         * null.
         */
        try {
            if (commandService != null) {
                StorageNodeId sni = storageNode.getStorageNodeId();
                StorageNodeParams param = commandService.getParameters()
                                                        .get(sni);
                return param.getRootDirPath();
            }
        } catch (Exception ignore) {
            return null;
        }
        return null;
    }

    /**
     * Get Login Manager to login kvstore
     *
     * @param loginMgrType login manager type
     * @return an instance of LoginManager when kvstore is secured; or null
     * @throws Exception
     */
    private LoginManager getLoginManager(LoginManagerType loginMgrType)
        throws Exception {
        if (securityFile != null) {
            KVStoreLogin storeLogin = new KVStoreLogin(user, securityFile);
            storeLogin.loadSecurityProperties();
            storeLogin.prepareRegistryCSF();

            LoginManager loginMgr = null;

            /* Needs authentication */
            if (storeLogin.foundSSLTransport()) {
                try {
                    final LoginCredentials creds =
                        storeLogin.makeShellLoginCredentials();
                    if (loginMgrType == LoginManagerType.REPNODELOGIN) {
                        loginMgr = KVStoreLogin.getRepNodeLoginMgr(host, port,
                                                                   creds, null);
                    } else if (loginMgrType == LoginManagerType.ADMINLOGIN) {
                        loginMgr = KVStoreLogin.getAdminLoginMgr(host, port,
                                                                 creds);
                    }
                } catch (AuthenticationFailureException afe) {
                    throw new Exception("Login failed: " + afe.getMessage());
                } catch (IOException ioe) {
                    throw new Exception("Failed to get login "
                            + "credentials: " + ioe.getMessage());
                }
            }
            return loginMgr;
        }

        /* LoginManager is null when kvstore is non-security */

        return null;
    }
}
