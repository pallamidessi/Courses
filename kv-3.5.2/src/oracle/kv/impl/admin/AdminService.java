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

import java.net.InetSocketAddress;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.HashSet;
import java.util.Properties;
import java.util.Set;
import java.util.StringTokenizer;
import java.util.logging.Level;
import java.util.logging.Logger;

import oracle.kv.impl.admin.param.AdminParams;
import oracle.kv.impl.admin.param.BootstrapParams;
import oracle.kv.impl.admin.param.GlobalParams;
import oracle.kv.impl.admin.param.SecurityParams;
import oracle.kv.impl.admin.param.StorageNodeParams;
import oracle.kv.impl.client.admin.ClientAdminService;
import oracle.kv.impl.fault.ProcessFaultHandler;
import oracle.kv.impl.measurement.ServiceStatusChange;
import oracle.kv.impl.mgmt.AdminStatusReceiver;
import oracle.kv.impl.param.LoadParameters;
import oracle.kv.impl.param.ParameterListener;
import oracle.kv.impl.param.ParameterMap;
import oracle.kv.impl.param.ParameterState;
import oracle.kv.impl.param.ParameterUtils;
import oracle.kv.impl.security.ConfigurationException;
import oracle.kv.impl.security.RoleResolver;
import oracle.kv.impl.security.SecureProxy;
import oracle.kv.impl.security.login.InternalLoginManager;
import oracle.kv.impl.security.login.UserLogin;
import oracle.kv.impl.topo.AdminId;
import oracle.kv.impl.topo.AdminType;
import oracle.kv.impl.topo.StorageNodeId;
import oracle.kv.impl.util.ConfigurableService;
import oracle.kv.impl.util.FileNames;
import oracle.kv.impl.util.PortRange;
import oracle.kv.impl.util.registry.RMISocketPolicy;
import oracle.kv.impl.util.registry.RMISocketPolicy.SocketFactoryPair;
import oracle.kv.impl.util.registry.RegistryUtils;
import oracle.kv.impl.util.server.LoggerUtils;

import com.sleepycat.je.rep.ReplicatedEnvironment.State;
import com.sleepycat.je.rep.ReplicationGroup;
import com.sleepycat.je.rep.ReplicationNetworkConfig;
import com.sleepycat.je.rep.util.ReplicationGroupAdmin;
import com.sleepycat.je.rep.utilint.HostPortPair;

/**
 * AdminService houses the two services provided for administering a kv store
 * instance and providing clients with access to Admin API functions.
 */
public class AdminService implements ConfigurableService {

    private AdminServiceParams params;
    private Admin admin = null;           /* Admin can be null at bootstrap. */
    private CommandService commandService;
    private CommandService exportableCommandService;
    private LoginService loginService;
    private UserLogin exportableUL;
    private ClientAdminService clientService;
    private ClientAdminService exportableClientService;
    private WebService webService;
    private AdminStatusReceiver statusReceiver = null;
    private ParameterListener parameterListener = null;
    private AdminSecurity adminSecurity;

    private Logger logger;

    private AdminServiceFaultHandler faultHandler;
    private final boolean usingThreads;

    /* Default Java heap to 96M for now */
    public static final String DEFAULT_JAVA_ARGS =
        "-XX:+DisableExplicitGC -Xms96M -Xmx128M " +
        "-XX:+HeapDumpOnOutOfMemoryError " +
        "-server" +

        /*
         * Disable JE's requirement that helper host names be resolvable.  We
         * want nodes to be able to start up even if other nodes in the
         * replication group have been removed and no longer have DNS names.
         * [#23120]
         */
        " -Dje.rep.skipHelperHostResolution=true";

    /* For unit test support, to determine if the service is up or down. */
    private boolean active = false;

    /**
     * Creates a non-bootstrap AdminService.  The initialize() method must
     * be called before start().
     */
    public AdminService(boolean usingThreads) {
        this.usingThreads = usingThreads;
        faultHandler = null;
    }

    /**
     * Creates a bootstrap AdminService.  The initialize() method should not
     * be called before start(). The bootstrap service is always created as
     * a PRIMARY Admin.
     */
    public AdminService(BootstrapParams bp,
                        SecurityParams sp,
                        boolean usingThreads) {
        this.usingThreads = usingThreads;
        deriveASParams(bp, sp);

        /*
         * No need to worry about RMI Socket Policies and Registry CSF here
         * because ManagedBootstrapAdmin takes care of that for us, if needed.
         */

        /* When kvStoreName is null, we are starting in bootstrap mode. */
        if (params.getGlobalParams().getKVStoreName() == null) {
            logger =
                LoggerUtils.getBootstrapLogger(bp.getRootdir(),
                                               FileNames.BOOTSTRAP_ADMIN_LOG,
                                               "BootstrapAdmin");
        } else {
            logger = LoggerUtils.getLogger(this.getClass(), params);
        }
        faultHandler = new AdminServiceFaultHandler(logger, this);
        adminSecurity = new AdminSecurity(this, logger);
    }

    /**
     * Initialize an AdminService.  This must be called before start() if not
     * created via the BootStrap constructor.
     */
    public void initialize(SecurityParams securityParams,
                           AdminParams adminParams,
                           LoadParameters lp) {

        securityParams.initRMISocketPolicies();

        GlobalParams globalParams =
            new GlobalParams(lp.getMap(ParameterState.GLOBAL_TYPE));
        StorageNodeParams storageNodeParams =
            new StorageNodeParams(lp.getMap(ParameterState.SNA_TYPE));

        params = new AdminServiceParams
            (securityParams, globalParams, storageNodeParams, adminParams);

        logger = LoggerUtils.getLogger(this.getClass(), params);
        if (faultHandler == null) {
            faultHandler = new AdminServiceFaultHandler(logger, this);
        }

        if (!usingThreads) {
            storageNodeParams.setRegistryCSF(securityParams);
        }

        adminSecurity = new AdminSecurity(this, logger);
    }

    /**
     * Start and stop are synchronized to avoid having stop() run before
     * start() is done.  This comes up in testing.
     */
    @Override
    public synchronized void start() {
        getFaultHandler().execute
            (new ProcessFaultHandler.SimpleProcedure() {

                @Override
                public void execute() {
                    startInternal();
                }
             });
    }

    private void startInternal() {

        final StorageNodeParams snParams = params.getStorageNodeParams();
        final String hostName = snParams.getHostname();

        /* Sets the hostname to be associated with rmi stubs. */
        System.setProperty("java.rmi.server.hostname", hostName);
        /* Disable to allow for faster timeouts on failed connections. */
        System.setProperty("java.rmi.server.disableHttp", "true");

        /*
         * If kvStoreName is null, then we are starting in bootstrap mode.
         * The Admin can't be created yet.
         */
        final String kvStoreName = params.getGlobalParams().getKVStoreName();
        if (kvStoreName == null) {
            logger.info("Starting in bootstrap mode");
        } else {
            logger.info("Starting AdminService");
            admin = new Admin(params, this);
        }

        /*
         * Create the UserLoginImpl instance and bind it in the registry if
         * security is enabled.
         */
        if (params.getSecurityParams().isSecure()) {

            ServiceBinder<UserLogin, LoginService> serviceBinder =
                new ServiceBinder<UserLogin, LoginService>
                (GlobalParams.ADMIN_LOGIN_SERVICE_NAME, this) {

                @Override
                LoginService makeInsecureService() {
                    return new LoginService(aservice);
                }

                @Override
                UserLogin getRemote() {
                    return insecureService.getUserLogin();
                }
            };

            loginService = serviceBinder.getInsecureService();
            exportableUL = serviceBinder.getSecureService();

            /* Install the login updater now */
            if (admin != null) {
                admin.installSecurityUpdater();
            }
        }

        /* Create the CommandService instance and bind it in the registry */
        ServiceBinder<CommandService, CommandService> serviceBinder =
            new ServiceBinder<CommandService, CommandService>
            (GlobalParams.COMMAND_SERVICE_NAME, this) {

            @Override
            CommandServiceImpl makeInsecureService() {
                return new CommandServiceImpl(aservice);
            }

            @Override
            CommandService getRemote() {
                return insecureService;
            }
        };
        commandService = serviceBinder.getInsecureService();
        exportableCommandService = serviceBinder.getSecureService();

        /* Create the ClientAdminService for handling DDL and DML statements */
        ServiceBinder<ClientAdminService, ClientAdminService> stmtBinder =
            new ServiceBinder<ClientAdminService, ClientAdminService>
            (GlobalParams.CLIENT_ADMIN_SERVICE_NAME, this) {

            @Override
            ClientAdminServiceImpl makeInsecureService() {
                return new ClientAdminServiceImpl(aservice);
            }

            @Override
            ClientAdminService getRemote() {
                return insecureService;
            }
        };
        clientService = stmtBinder.getInsecureService();
        exportableClientService = stmtBinder.getSecureService();

        /* Create and start the WebService instance. */
        final int httpPort = params.getAdminParams().getHttpPort();
        if (httpPort != 0 && kvStoreName != null) {
            logger.info("Starting Web service on port " + httpPort);
            startWebService();
        }

        synchronized (this) {
            active = true;
            this.notifyAll();
        }
        logger.info("Started AdminService");
    }

    private void startWebService() {
        try {
            /* TODO: provide a non-internal, non-admin login */
            webService =
                new WebService(CommandServiceAPI.wrap(commandService, null),
                               params, logger);
                webService.start();
        } catch (Exception e) {
            String msg = "Starting WebService failed. ";
            logger.severe(msg + LoggerUtils.getStackTrace(e));

            /*
             * Don't throw here, since CommandService is up, it could still
             * be a useful session.
             */
        }
    }

    @Override
    public synchronized void stop(boolean force) {
        logger.info("Shutting down AdminService instance" +
                    (force ? " (force)" : ""));

        updateAdminStatus(admin, ServiceStatus.STOPPING);
        String hostName = params.getStorageNodeParams().getHostname();
        if (commandService != null) {
            ((CommandServiceImpl) commandService).stopRemoteTestInterface(
                logger);
            try {
                logger.info("Unbinding CommandService");
                RegistryUtils.unbind
                    (hostName,
                     params.getStorageNodeParams().getRegistryPort(),
                     GlobalParams.COMMAND_SERVICE_NAME,
                     exportableCommandService);
                commandService = null;
            } catch (RemoteException re) {
                String msg = "Can't unbind CommandService. ";
                logger.severe(msg + LoggerUtils.getStackTrace(re));
                throw new IllegalStateException(msg, re);
            }
        }

        if (clientService != null) {
            try {
                logger.info("Unbinding ClientAdminService");
                RegistryUtils.unbind
                    (hostName,
                     params.getStorageNodeParams().getRegistryPort(),
                     GlobalParams.CLIENT_ADMIN_SERVICE_NAME,
                     exportableClientService);
                clientService = null;
            } catch (RemoteException re) {
                String msg = "Can't unbind ClientAdminService. ";
                logger.severe(msg + LoggerUtils.getStackTrace(re));
                throw new IllegalStateException(msg, re);
            }
        }

        if (loginService != null) {
            try {
                logger.info("Unbinding LoginService");
                RegistryUtils.unbind
                    (hostName,
                     params.getStorageNodeParams().getRegistryPort(),
                     GlobalParams.ADMIN_LOGIN_SERVICE_NAME,
                     exportableUL);
                loginService = null;
            } catch (RemoteException re) {
                String msg = "Can't unbind LoginService. ";
                logger.severe(msg + LoggerUtils.getStackTrace(re));
                throw new IllegalStateException(msg, re);
            }
        }

        if (webService != null) {
            logger.info("Shutting down WebService");
            try {
                webService.stop();
                webService = null;
            } catch (Exception e) {
                String msg = "Can't stop WebService. ";
                logger.severe(msg + LoggerUtils.getStackTrace(e));
                throw new IllegalStateException(msg, e);
            }
        }

        if (admin != null) {
            logger.info("Shutting down Admin");
            admin.shutdown(force);
            admin = null;
        }

        active = false;
        this.notifyAll();
    }

    /**
     *  Wait for the service to be started or stopped.
     */
    public synchronized void waitForActive(boolean desiredState)
        throws InterruptedException {

        while (active != desiredState) {
            this.wait();
        }
    }

    /**
     * Accessor for admin, which can be null.
     */
    public Admin getAdmin() {
        return admin;
    }

    /**
     * Accessor for params
     */
    public AdminServiceParams getParams() {
        return params;
    }

    /**
     * Accessor for security info
     */
    public AdminSecurity getAdminSecurity() {
        return adminSecurity;
    }

    /**
     * Accessor for login services
     */
    public LoginService getLoginService() {
        return loginService;
    }

    /**
     * Accessor for internal login manager
     */
    public InternalLoginManager getLoginManager() {
        return (adminSecurity == null) ? null : adminSecurity.getLoginManager();
    }

    /**
     * Configure the store name and then create the Admin instance, which
     * creates the Admin database.  This method can be used only when the
     * AdminService is running in bootstrap/configuration mode.
     */
    public void configure(String storeName) {
        assert admin == null;
        params.getGlobalParams().setKVStoreName(storeName);

        /*
         * Since we are bootstrapping, there is a chicken-egg problem regarding
         * the HA service port.  The bootstrap parameters have an HA port range
         * configured for the SNA.  We know that none of these are in use at
         * this time, so we will commandeer the first port in this range for
         * now.  Later, when this #1 admin is officially deployed via the
         * deployment plan, we will note the use of this port in the Admin's
         * parameter record, thereby reserving its use with the PortTracker.
         */
        final StorageNodeParams snp = params.getStorageNodeParams();

        final int haPort = PortRange.getRange(snp.getHAPortRange()).get(0);
        AdminParams ap = params.getAdminParams();
        ap.setJEInfo(snp.getHAHostname(), haPort, snp.getHAHostname(), haPort);

        admin = new Admin(params, this);

        /* Now we can use the real log configuration. */
        logger.info("Changing log files to log directory for store " +
                    storeName);
        logger = LoggerUtils.getLogger(this.getClass(), params);
        faultHandler.setLogger(logger);
        adminSecurity.configure(storeName);
        if (loginService != null) {
            loginService.resetLogger(logger);
        }
        logger.info("Configured Admin for store: " + storeName);

        /* We can install the login updater now */
        if (params.getSecurityParams() != null) {
            admin.installSecurityUpdater();
        }

        /* Reset logger for web service if web service is already started,
         * if not start web service now.
         */
        if (webService != null) {
            webService.resetLog(logger);
        } else {
            /* Create and start the WebService instance. */
            final int httpPort = params.getAdminParams().getHttpPort();
            if (httpPort != 0 && storeName != null) {
                logger.info("Starting Web service on port " + httpPort);
                startWebService();
            }
        }
    }
    /**
     * Subordinate services (CommandService and WebService) use this
     * method when logging.  AdminService's logger can change during
     * bootstrap.
     */
    public Logger getLogger() {
        return logger;
    }

    /**
     * Returns the fault handler associated with the service
     */
    public ProcessFaultHandler getFaultHandler() {
       return faultHandler;
    }

    public boolean getUsingThreads() {
        return usingThreads;
    }

    /**
     * Initialize our AdminServiceParams member based on the contents of a
     * BootParams instance.
     */
    private void deriveASParams(BootstrapParams bp, SecurityParams sp) {

        String storeName = bp.getStoreName();
        StorageNodeId snid =
            new StorageNodeId(storeName == null ? 1 : bp.getId());

        GlobalParams gp = new GlobalParams(storeName);

        /*
         * Pass user-defined external authentication method to global parameter
         * so that bootstrap admin can recognize this setting automatically.
         */
        gp.setUserExternalAuthMethods(bp.getUserExternalAuth());

        StorageNodeParams snp =
            new StorageNodeParams(snid, bp.getHostname(), bp.getRegistryPort(),
                                  "Admin Bootstrap");

        snp.setRootDirPath(bp.getRootdir());
        snp.setHAHostname(bp.getHAHostname());
        snp.setHAPortRange(bp.getHAPortRange());
        snp.setServicePortRange(bp.getServicePortRange());

        final AdminParams ap = new AdminParams(new AdminId(1),
                                               snp.getStorageNodeId(),
                                               bp.getAdminHttpPort(),
                                               AdminType.PRIMARY);
        params = new AdminServiceParams(sp, gp, snp, ap);
    }

    /**
     * Issue a BDBJE update of the target node's HA address.
     */
    public void updateMemberHAAddress(AdminId targetId,
                                      String targetHelperHosts,
                                      String newNodeHostPort) {

        /*
         * Setup the helper hosts to use for finding the master to execute this
         * update.
         */
        Set<InetSocketAddress> helperSockets = new HashSet<InetSocketAddress>();
        StringTokenizer tokenizer =
                new StringTokenizer(targetHelperHosts,
                                    ParameterUtils.HELPER_HOST_SEPARATOR);
        while (tokenizer.hasMoreTokens()) {
            String helper = tokenizer.nextToken();
            helperSockets.add(HostPortPair.getSocket(helper));
        }

        String storeName = params.getGlobalParams().getKVStoreName();
        String groupName = Admin.getAdminRepGroupName(storeName);

        /* Change the target node's HA address. */
        logger.info("Updating rep group " + groupName + " using helpers " +
                    targetHelperHosts + " to change " + targetId + " to " +
                    newNodeHostPort);

        String targetNodeName = Admin.getAdminRepNodeName(targetId);

        /*
         * Figure out the right ReplicationNetworkConfig to use.  If there's
         * an admin present, we just use that config.  Otherwise (not sure
         * why it wouldn't be if we are part of a replicated config),
         * construct a DataChannelFactory from the SecurityParams, if
         * present.
         */
        final ReplicationNetworkConfig repNetConfig;
        if (admin != null) {
            repNetConfig = admin.getRepNetConfig();
        } else if (params.getSecurityParams() == null) {
            repNetConfig = null;
        } else {
            final Properties haProps =
                params.getSecurityParams().getJEHAProperties();
            logger.info("DataChannelFactory: " +
                        haProps.getProperty(
                            ReplicationNetworkConfig.CHANNEL_TYPE));
            repNetConfig = ReplicationNetworkConfig.create(haProps);
        }

        ReplicationGroupAdmin rga =
            new ReplicationGroupAdmin(groupName, helperSockets, repNetConfig);

        ReplicationGroup rg = rga.getGroup();
        com.sleepycat.je.rep.ReplicationNode jeRN =
            rg.getMember(targetNodeName);
        if (jeRN == null) {
            throw new IllegalStateException
                (targetNodeName + " does not exist in replication group " +
                 groupName);
        }

        String newHostName =  HostPortPair.getHostname(newNodeHostPort);
        int newPort =  HostPortPair.getPort(newNodeHostPort);

        if ((jeRN.getHostName().equals(newHostName)) &&
            (jeRN.getPort() == newPort)) {

            /*
             * This node is already changed, nothing more to do. Do this
             * check in case the change has been made previously, and this
             * node is alive, as the updateAddress() call will incur an
             * exception if the node is alive.
             */
            return;
        }

        rga.updateAddress(targetNodeName, newHostName, newPort);
    }

    public void installStatusReceiver(AdminStatusReceiver asr) {

        statusReceiver = asr;

        if (admin == null) {
            /* We're unconfigured; report waiting for deployment. */
            updateAdminStatus(null, ServiceStatus.WAITING_FOR_DEPLOY);
        } else {
            /* Otherwise, if we're up and servicing calls, we are running. */
            updateAdminStatus(admin, ServiceStatus.RUNNING);
        }
    }

    void updateAdminStatus(Admin a, ServiceStatus newStatus) {
        if (statusReceiver == null) {
            return;
        }

        /*
         * During bootstrapping, we have no Admin, so we can't reliably add the
         * ParameterListener when installing the receiver.  This method is
         * called at receiver installation time, and also when the Admin's
         * status changes.  When it changes from bootstrap to configured mode,
         * we can install the listener here.  Also, the Admin is given as an
         * argument to this method because, during bootstrapping, this method
         * is called before AdminService's admin instance variable is assigned.
         */
        if (a != null && parameterListener == null) {
            parameterListener = new ParameterChangeListener();
            a.addParameterListener(parameterListener);
            /* Prime the pump with the first newParameters call. */
            parameterListener.newParameters
                (null,
                 a.getParams().getAdminParams().getMap());
        }

        State adminState = (a == null ? null : a.getReplicationMode());

        boolean isMaster =
            (adminState == null || adminState != State.MASTER ? false : true);

        try {
            statusReceiver.updateAdminStatus
                (new ServiceStatusChange(newStatus), isMaster);
        } catch (RemoteException e) {
            /*
             * This should not prevent the admin from coming up.  Just log the
             * failure.
             */
            logger.log
                (Level.WARNING,
                 "Failed to send status updateof " + newStatus.toString() +
                 " to MgmtAgent", e);
        }
    }

    /* For test purpose */
    WebService getWebService() {
        return this.webService;
    }

    public RoleResolver getRoleResolver() {
        return (adminSecurity == null) ? null : adminSecurity.getRoleResolver();
    }

    /**
     * The parameter listener for updating the status receiver.
     */
    private class ParameterChangeListener implements ParameterListener {

        @Override
        public void newParameters(ParameterMap oldMap, ParameterMap newMap) {
            try {
                statusReceiver.receiveNewParams(newMap);
            } catch (RemoteException re) {
                /* If we fail to deliver, who can we tell about it? */
                logger.log
                    (Level.WARNING,
                     "Failure to deliver parameter change to MgmtAgent", re);
                return;
            }
        }
    }

    /**
     * Helper class to create and then export a secured RMI service.
     */
    private abstract class ServiceBinder<S extends Remote, I> {
        private final String svcName;
        protected final AdminService aservice;
        protected I insecureService;
        private S secureService;

        /**
         * Instantiating a ServiceBinder will actually invoke the creation and
         * proxy wrapping of the targer service.
         *
         * @param svcName is the RMI service name.
         */
        ServiceBinder(String svcName, AdminService aservice) {
            this.svcName = svcName;
            this.aservice = aservice;

            /* create and wrap the RMI service */
            initService();
        }

        public S getSecureService() {
            return secureService;
        }

        public I getInsecureService() {
            return insecureService;
        }

        /** Instantiate an insecure version of the service */
        abstract I makeInsecureService();

        /** Return the Remote from the insecure version of the service. */
        abstract S getRemote();

        /**
         * Start the service, and wrap it with the appropriate security proxy
         */
        private void initService() {
            final StorageNodeParams snParams = params.getStorageNodeParams();
            final String hostName = snParams.getHostname();
            final int registryPort = snParams.getRegistryPort();
            final String kvStoreName =
                params.getGlobalParams().getKVStoreName();

            logger.info("Starting " + svcName + " on rmi://" + hostName + ":" +
                        registryPort + "/" + svcName);
            try {
                insecureService = makeInsecureService();

                /* Wrap the insecure service in a secure proxy */
                try {
                    secureService = SecureProxy.create
                        (getRemote(),
                         adminSecurity.getAccessChecker(),
                         faultHandler);
                    logger.info("Successfully created a secure proxy for " +
                                svcName);
                } catch (ConfigurationException ce) {
                    throw new IllegalStateException
                        ("Unable to create a secure proxy for " + svcName, ce);
                }

                RMISocketPolicy rmiSocketPolicy =
                    params.getSecurityParams().getRMISocketPolicy();
                final SocketFactoryPair sfp =
                    params.getStorageNodeParams().getAdminCommandServiceSFP
                    (rmiSocketPolicy, kvStoreName);

                RegistryUtils.rebind(hostName,
                                     registryPort,
                                     svcName,
                                     secureService,
                                     sfp.getClientFactory(),
                                     sfp.getServerFactory());

            } catch (RemoteException re) {
                String msg = "Starting " + svcName + " failed";
                logger.severe(msg + LoggerUtils.getStackTrace(re));
                throw new IllegalStateException(msg, re);
            }
        }
    }
}
