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

package oracle.kv.util;

import static oracle.kv.impl.util.JsonUtils.createObjectNode;
import static oracle.kv.impl.util.JsonUtils.createWriter;
import static oracle.kv.impl.util.JsonUtils.getArray;

import java.io.IOException;
import java.io.PrintStream;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.Registry;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import oracle.kv.LoginCredentials;
import oracle.kv.KVStoreException;
import oracle.kv.impl.admin.AdminStatus;
import oracle.kv.impl.admin.CommandJsonUtils;
import oracle.kv.impl.admin.CommandResult;
import oracle.kv.impl.admin.CommandServiceAPI;
import oracle.kv.impl.admin.param.AdminParams;
import oracle.kv.impl.admin.param.GlobalParams;
import oracle.kv.impl.admin.param.Parameters;
import oracle.kv.impl.monitor.views.ServiceChange;
import oracle.kv.impl.rep.RepNodeStatus;
import oracle.kv.impl.security.login.LoginManager;
import oracle.kv.impl.security.util.KVStoreLogin;
import oracle.kv.impl.sna.StorageNodeStatus;
import oracle.kv.impl.topo.AdminId;
import oracle.kv.impl.topo.Datacenter;
import oracle.kv.impl.topo.RepGroup;
import oracle.kv.impl.topo.RepGroupId;
import oracle.kv.impl.topo.RepNode;
import oracle.kv.impl.topo.RepNodeId;
import oracle.kv.impl.topo.ResourceId;
import oracle.kv.impl.topo.StorageNode;
import oracle.kv.impl.topo.StorageNodeId;
import oracle.kv.impl.topo.Topology;
import oracle.kv.impl.util.CommandParser;
import oracle.kv.impl.util.ConfigurableService.ServiceStatus;
import oracle.kv.impl.util.FormatUtils;
import oracle.kv.impl.util.HostPort;
import oracle.kv.impl.util.TopologyLocator;
import oracle.kv.impl.util.registry.RegistryUtils;
import oracle.kv.util.PingCollector.AdminInfo;
import oracle.kv.util.shell.Shell;

import org.codehaus.jackson.JsonNode;
import org.codehaus.jackson.map.ObjectWriter;
import org.codehaus.jackson.node.ArrayNode;
import org.codehaus.jackson.node.ObjectNode;

/**
 * Pings all the RNs, SNs, and admins associated with a KVS. It is provided a
 * single storage node's host:port or a list of host:port pairs, and uses those
 * to find an RMI Registry and uses that to locate a service (RepNode or Admin)
 * that can provide a store topology.  The topology is then used to obtain
 * knowledge about all the other nodes in the store.  If an admin can be found,
 * then it is used to obtain parameters which are used to find the other
 * admins.
 *
 * Description of flags:
 *  (from CommandParser):
 *  -host -- single host to use.  Must not be used if -helper-hosts is used.
 *  -port -- port to use on the single host  Must not be used if -helper-hosts
 *    is used.
 *  -helper-hosts -- a comma-separated list of host:port pairs.  This allows
 *    the call to be more resilient to the case where an individual host may
 *    not be accessible.  Must not be used if -host and -port are specified.
 *
 * <p>Ping also provides utility methods to other functionality that needs to
 * ping remote services.
 *
 * <p>Here is an annotated example of the output of the ping command in JSON
 * format:
 *
 * <pre>
 * {
 *   // Topology overview
 *   "topology" : {
 *     "storeName" : "mykvstore",
 *     "sequenceNumber" : 21,
 *     "numPartitions" : 10,
 *     "numStorageNodes" : 2,
 *     "time" : 1425658621417,
 *     "version" : "12.1.3.2.15"
 *   },
 *
 *   // Overview of status of all shards in the store
 *   "shardStatus" : {
 *     "healthy" : 2,           // Shards with all RNs active
 *     "writable-degraded" : 0, // Some inactive RNs but with quorum
 *     "read-only" : 0,         // Lost quorum but with some active RNs
 *     "offline" : 0            // No active RNs
 *   },
 *
 *   // Overview of the status of the admin, one of "healthy",
 *   // "writable-degraded", "read-only", or "offline"
 *   "adminStatus" : "healthy",
 *
 *   // Status of each zone
 *   "zoneStatus" : [ {
 *     "resourceId" : "zn1",
 *     "name" : "Boston",
 *     "type" : "PRIMARY",
 *     "rnSummaryStatus" : {
 *       "online" : 6,          // RNs in the zone that are online
 *       "offline" : 0,         // RNs in the zone that are offline
 *       "hasReplicas" : true,  // Whether the zone has non-master replicas
 *
 *       // The maximum over all replicas in the zone of the estimated delay,
 *       // in milliseconds, between when a transaction was committed on the
 *       // master and when the master learned that the transaction was
 *       // processed on a replica.
 *       "maxDelayMillis" : 1,
 *
 *       // The maximum over all replicas in the zone of the estimated time, in
 *       // seconds, until a replica eliminate delays with the master. Uses the
 *       // maximum positive long value for replicas that are remaining behind
 *       // without catching up or falling behind.  For replicas that are
 *       // falling behind, uses a negative value whose absolute value is the
 *       // estimated time until the delay doubles. If there are negative
 *       // values, then the maximum is the negative value nearest to zero,
 *       // which represents the replica that is falling behind most quickly.
 *       "maxCatchupTimeSecs" : 0
 *     }
 *   } ],
 *
 *   // Status of each storage node
 *   "snStatus" : [ {
 *     "resourceId" : "sn1",
 *     "hostname" : "localhost",
 *     "registryPort" : 5001,
 *     "zone" : {
 *       "resourceId" : "zn1",
 *       "name" : "Boston",
 *       "type" : "PRIMARY"     // PRIMARY or SECONDARY
 *     },
 *
 *     // The service status of an admin or replication node, with values from
 *     // the constant values of the ConfigurableService.ServiceStatus enum.
 *     // One of "STARTING", "WAITING_FOR_DEPLOY", "RUNNING", "STOPPING",
 *     // "STOPPED", "ERROR_RESTARTING", "ERROR_NO_RESTART", or "UNREACHABLE".
 *     "serviceStatus" : "RUNNING",
 *
 *     "version" :
 *     "12cR1.3.2.15 2015-03-06 04:16:48 UTC  Build id: 65a27a9d8a1b+",
 *
 *     // Status of the admin node on this storage node, missing if no admin
 *     "adminStatus" : {
 *       "resourceId" : "admin1",
 *       "status" : "RUNNING",
 *
 *       // The replication state of the node, with values from the constant
 *       // values of the ReplicatedEnvironment.State enum.  One of "DETACHED",
 *       // "UNKNOWN", "MASTER", or "REPLICA".
 *       "state" : "MASTER"
 *
 *       // If the node is the master, whether the node is known to be the
 *       // authoritative master: one that is in active contact with a quorum
 *       // of replicas
 *       "authoritativeMaster" : true;
 *     },
 *
 *     // Status of each replication node
 *     "rnStatus" : [ {
 *       "resourceId" : "rg1-rn1",
 *       "status" : "RUNNING",
 *       "state" : "MASTER",
 *       "authoritativeMaster" : true;
 *       "sequenceNumber" : 37,
 *       "haPort" : 5003
 *     }, {
 *       "resourceId" : "rg1-rn3",
 *       "status" : "RUNNING",
 *       "state" : "REPLICA",
 *       "sequenceNumber" : 37,
 *       "haPort" : 5005,
 *
 *       // Whether the node is performing a network restore
 *       "networkRestoreUnderway" : false
 *
 *       // The estimated delay, in milliseconds, between when a transaction
 *       // was committed on the master and when the master learned that the
 *       // transaction was processed on the replica.  Missing if this node is
 *       // a master or if the value is not known.
 *       "delayMillis" : 0,
 *
 *       // The estimated time, in seconds, until all of the replica eliminate
 *       // its delay with the master. Set to the maximum positive long value
 *       // if the replica is not catching up. For replicas that are falling
 *       // behind, set to a negative value whose absolute value is the
 *       // estimated time until the delay doubles.  Missing if this node is a
 *       // master or if the value is not known.
 *       "catchupTimeSecs" : 0,
 *
 *       // The estimated rate, in milliseconds per minute, that the replica is
 *       // reducing its delay with the master.  Set to a negative value if the
 *       // replica is falling behind.  Missing if this node is a master or if
 *       // the value is not known.
 *       "catchupRateMillisPerMinute" : 0
 *     },
 *     // ...
 *   },
 *   //...
 *   ]
 *
 *   // Exit code and result code for command is displayed in the json
 *   // report as well as being used as the process exit code. See a
 *   // description of the exit values below
 *    "operation" : "ping",
 *    "return_code" : 5000,
 *    "description" : "No errors found",
 *    "exit_code" : 0
 *   }
 * }
 * </pre>
 * <p>
 * The process exit code can be used as a diagnostic to determine the state of
 * the store. Each exit code is paired with a NoSQL error code, as defined in
 * the NoSQL error messages catalog. TBW - write up the exit codes.
 * The exit codes have the following meaning:
 *
 * <p>
 * 0 (EXIT_OK) -- all services in the store could be located and are in a
 *   known, good state (e.g. RUNNING).
 * <p>
 * 1 (EXIT_OPERATIONAL) -- one or more services in the store could not be
 *   reached, or are in an unknown or not usable state.  In this case the store
 *   should support all operations across all shards, as well as for the admin,
 *   but may be in a state of degraded performance.  Some action should be
 *   taken to find and fix the problem before part of the store becomes
 *   unavailable.
 * <p>
 * 2 (EXIT_NO_ADMIN_QUORUM) -- the admin replication group does not have
 *   quorum or is not available at all. The store supports all normal data
 *   operations despite the loss of admin quorum, but this state requires
 *   immediate attention to restore full store capabilities.
 * <p>
 * 3 (EXIT_NO_SHARD_QUORUM) -- one or more of the shards does not have quorum
 *   and either cannot accept write requests, or is completely unavailable.
 *   This state requires immediate attention to restore store capabilities.
 *   This exit code takes precedence over EXIT_NO_ADMIN_QUORUM, so if this
 *   exit code is used it is possible that the admin capabilities are also
 *   reduced or unavailable.
 * <p>
 * All of the last three of the exit codes above (1-3) could be indicative of a
 * network connectivity issue and that should be checked first, before
 * concluding that any services have a problem.
 * <p>
 * 100 (EXIT_USAGE) -- a usage error
 * <p>
 * 101 (EXIT_TOPOLOGY_FAILURE) -- Ping was unable to find a
 *   Topology in order to operate.  This could be a store problem, a network
 *   problem, or it could be a usage problem with the parameters passed to
 *   Ping (e.g. the host and port or helper-hosts list are not part of a store).
 * <p>
 * 102 (EXIT_UNEXPECTED) -- the utility has experienced an unexpected error.
 *
 * Note that each Ping instance accumulates state after each ping call
 * and should be used a single time.
 */
public class Ping {

    /* External commands, for "java -jar" usage. */
    public static final String COMMAND_NAME = "ping";
    public static final String COMMAND_DESC =
        "attempts to contact a store to get status of running services";
    private static final String HELPER_HOSTS_FLAG = "-helper-hosts";
    public static final String COMMAND_ARGS =
        CommandParser.getHostUsage() + " " +
        CommandParser.getPortUsage() + " or\n\t" +
        HELPER_HOSTS_FLAG + " <host:port[,host:port]*>\n\t" +
        CommandParser.getUserUsage() + "\n\t" +
        CommandParser.getSecurityUsage() + "\n\t" +
        CommandParser.optional(CommandParser.JSON_FLAG);
   static final String EXIT_CODE_FIELD = "exit_code";

    /*
     * The possible return codes for the Ping utility, obeying the Unix
     * convention that a non-zero value is an abnormal termination.
     */
    public static enum ExitCode {
        EXIT_OK(0, ErrorMessage.NOSQL_5000, "No errors found"),
        EXIT_OPERATIONAL(1, ErrorMessage.NOSQL_5301,
                         "Store is operational but some services are " +
                         "unavailable"),
        EXIT_NO_ADMIN_QUORUM(2, ErrorMessage.NOSQL_5302,
                             "All data operations are full available but " +
                             "administrative changes are disabled"),
        EXIT_NO_SHARD_QUORUM(3, ErrorMessage.NOSQL_5303,
                             "One or more shards cannot accept write " +
                             "operations"),
        EXIT_USAGE(100, ErrorMessage.NOSQL_5100,
                   "Usage error"),
        EXIT_TOPOLOGY_FAILURE(101, ErrorMessage.NOSQL_5304,
                              "Topology cannot be found"),
        EXIT_UNEXPECTED(102, ErrorMessage.NOSQL_5500, "Internal error");

        /* A value that is a valid Unix return code, in the range of 0-127 */
        private final int returnCode;

        /*
         * A value that is an Oracle NoSQL error code, which maps into the
         * error message catalog.
         */
        private final ErrorMessage errorCode;

        private final String description;

        ExitCode(int returnCode, ErrorMessage errorCode, String description) {
            this.returnCode = returnCode;
            this.errorCode = errorCode;
            this.description = description;
        }

        public int value(){
            return this.returnCode;
        }

        public ErrorMessage getErrorCode() {
            return errorCode;
        }

        public String getDescription() {
            return description;
        }
    }

    /** Returns the status for an Admin, or null if not known. */
    /* TODO: Replace with Function<AdminId, AdminStatus> in Java 8 */
    public interface AdminStatusFunction {
        AdminStatus get(AdminId adminId);
    }

    /** Returns the status for an RN, or null if not known. */
    /* TODO: Replace with Function<RepNode, RepNodeStatus> in Java 8 */
    public interface RepNodeStatusFunction {
        RepNodeStatus get(RepNode rn);
    }

    /* Ping will find a topology and params to direct its searches */
    private Topology topo;
    private final Parameters params;
    private final boolean verbose;
    private final boolean getJson;
    private final PrintStream ps;
    private final LoginManager adminLoginManager;

    /*
     * The problem report stores information that can be used for follow-on
     * action. For example, if a component in the store couldn't be contacted,
     * the problem report lists enough information so the caller can run a
     * network connectivity check against that hostport.
     */
    private ArrayList<Problem> problemReport = new ArrayList<Problem>();

    /*
     * exitCode holds the Unix system exitcode that will be returned by
     * Ping. It acts as a summary, or signal, of what happened in the call,
     * giving the user a basic green (ok), yellow (something wrong), red (big
     * problem) type health check. ExitCode should be set via any path that
     * leads to an exit. Initialize it here to indicate an unexpected, internal
     * error. If all goes as it should, the return code will be set to a proper
     * value by follow on processing. If there is a bug in the utility, and
     * the exitCode is not set, this initialization will point out the
     * problem.
     */
    private ExitCode exitCode = ExitCode.EXIT_UNEXPECTED;

    /**
     * Usage: java -jar KVHOME/lib/kvstore.jar ping
     *	-host <hostname> -port <port> or
     *	-helper-hosts <host:port[,host:port]*>
     *	-username <user>
     *	-security <security-file-path>
     *	[-json]
     */
    public static void main(String[] args) {

        class PingParser extends CommandParser {

            /*
             * This is an internal flag used by tests to allow them to
             * call Ping.main() without exiting.
             */
            private final String DONT_EXIT_FLAG = "-no-exit";
            private String helperHosts = null;
            private boolean dontExit = false;

            PingParser(String[] args1) {
                super(args1);
            }

            @Override
            public void usage(String errorMsg) {
                /*
                 * Note that you can't really test illegal arguments in a
                 * threaded unit test -- the call to exit(..) when
                 * dontExit is false doesn't kill the process, and the error
                 * message gets lost. Still worth using dontExit so the
                 * unit test process doesn't die, but unit testing of bad
                 * arg handling has to happen with a process.
                 */
                if (!getJson()) {
                    if (errorMsg != null) {
                        System.err.println(errorMsg);
                    }
                    System.err.println(KVSTORE_USAGE_PREFIX + COMMAND_NAME +
                                       "\n\t" + COMMAND_ARGS);
                }
                exit(dontExit, errorMsg, ExitCode.EXIT_USAGE, getJson(),
                     System.err);
            }

            @Override
            protected boolean checkArg(String arg) {
                if (arg.equals(HELPER_HOSTS_FLAG)) {
                    helperHosts = nextArg(arg);
                    return true;
                }

                if (arg.equals(DONT_EXIT_FLAG)) {
                    dontExit = true;
                    return true;
                }
                return false;
            }

            private String getHelperHosts() {
                return helperHosts;
            }

            private boolean getDontExit() {
                return dontExit;
            }

            @Override
            protected void verifyArgs() {
                /* Check that one or more helper hosts are supplied */
                if (getHelperHosts() != null &&
                    (getHostname() != null || (getRegistryPort() != 0))) {
                    usage("Only one of either " +  HELPER_HOSTS_FLAG + " or " +
                          HOST_FLAG + " plus " + PORT_FLAG +
                          "may be specified");
                }

                if (getHelperHosts() == null) {
                    if (getHostname() == null) {
                        missingArg(HOST_FLAG);
                    }
                    if (getRegistryPort() == 0) {
                        missingArg(PORT_FLAG);
                    }
                } else {
                    /*
                     * Helper hosts have been supplied - validate the
                     * argument.
                     */
                    try {
                        validateHelperHosts(getHelperHosts());
                    } catch (IllegalArgumentException e) {
                        usage("Illegal value for " + HELPER_HOSTS_FLAG );
                    }
                }
            }

            /**
             * Validate that each helper host entry in the form
             * <string>:<number>
             */
            private void validateHelperHosts(String helperHostVal)
                throws IllegalArgumentException {

                if (helperHostVal == null) {
                    throw new IllegalArgumentException
                        ("helper hosts cannot be null");
                }

                String[] hosts = helperHostVal.split(",");
                HostPort.parse(hosts);
            }

            /**
             * Return a list of hostport strings. Assumes that an argument
             * to helperHosts has already been validated.
             */
            List<String> createHostPortList() {
                String[] hosts = null;
                if (helperHosts != null) {
                    hosts = helperHosts.split(",");
                } else {
                    hosts = new String[1];
                    hosts[0] = getHostname() + ":" + getRegistryPort();
                }
                HostPort[] hps = HostPort.parse(hosts);
                List<String> hpList = new ArrayList<String>();
                for (HostPort hp : hps) {
                    hpList.add(hp.toString());
                }
                return hpList;
            }
        }

        PingParser pp = new PingParser(args);
        try {
            pp.parseArgs();
        } catch (Exception e) {
            final boolean json = Shell.checkArg(args, CommandParser.JSON_FLAG);
            exit(pp.getDontExit(),
                 "Argument error: " + e.getMessage(),
                 ExitCode.EXIT_USAGE,
                 json,
                 System.err);
            return;
        }

        final KVStoreLogin storeLogin =
            new KVStoreLogin(pp.getUserName(), pp.getSecurityFile());
        try {
            storeLogin.loadSecurityProperties();
        } catch (IllegalArgumentException iae) {
            exit(pp.getDontExit(),
                 iae.getMessage(),
                 ExitCode.EXIT_USAGE,
                 pp.getJson(),
                 System.err);
            return;
        }

        LoginCredentials loginCreds = null;
        if (storeLogin.foundSSLTransport()) {
            storeLogin.prepareRegistryCSF();
            try {
                loginCreds = storeLogin.makeShellLoginCredentials();
            } catch (IOException ioe) {
                exit(pp.getDontExit(),
                     "Failed to get login credentials: " + ioe.getMessage(),
                     ExitCode.EXIT_USAGE,
                     pp.getJson(),
                     System.err);
                return;
            }
        }

        Ping ping = null;
        List<String> hostports = pp.createHostPortList();
        try {
            ping = new Ping(hostports,
                            pp.getVerbose(),
                            pp.getJson(),
                            System.err,
                            loginCreds);
        } catch (KVStoreException e) {
            /*
             * Has problems getting the topology or params.
             * For backward compatibility, see if we should generate the old
             * no-SNA-exists but isn't deployed yet message. Remove this when
             * the SNA status command is implemented that will give the user a
             * way to get a more understandable message that says the SNA
             * exists, but isn't yet deployed.
             */
            if (!pp.getJson()) {
                /* Remove this when SNA status exists! */
                checkIfSNAIsDeployed(hostports.get(0));
            }

            exit(pp.getDontExit(),
                 "Can't find store topology: " + e.getMessage(),
                 ExitCode.EXIT_TOPOLOGY_FAILURE,
                 pp.getJson(),
                 System.err);
            return;
        }

        /*
         * Ping all the components in the store, analyze the status, and
         * display the results.
         */
        ping.pingTopology();

        /*
         * Be sure not to specify an exit message here - all information
         * should have been displayed via pingTopology.
         */
        exitNoDisplay(pp.getDontExit(), ping.exitCode);
    }

    /**
     * For use cases where the caller has a topology in hand already
     */
    private Ping(Topology topo,
                 Parameters params,
                 boolean verbose,
                 boolean getJson,
                 PrintStream ps,
                 LoginManager adminLoginManager) {
        this.topo = topo;
        this.params = params;
        this.verbose = verbose;
        this.getJson = getJson;
        this.ps = ps;
        this.adminLoginManager = adminLoginManager;
    }

    /**
     * For use cases where the utility must find a topology.
     * @throws KVStoreException 
     */
    private Ping(List<String> hostPorts,
                 boolean verbose,
                 boolean getJson,
                 PrintStream ps,
                 LoginCredentials loginCreds)
        throws KVStoreException {
        this.verbose = verbose;
        this.getJson = getJson;
        this.ps = ps;

        /* Search available SNs for a topology */
        String[] hostPortsArray = new String[hostPorts.size()];
        hostPortsArray = hostPorts.toArray(hostPortsArray);
        if (loginCreds == null) {
            adminLoginManager = null;
        } else {
            adminLoginManager = KVStoreLogin.getAdminLoginMgr(hostPortsArray,
                                                             loginCreds);
        }

        try {
            topo = TopologyLocator.get(hostPortsArray, 0, 
                                       adminLoginManager, null);
        } catch (KVStoreException topoLocEx) {
            /* had a problem getting a topology - try using the Admins */
            if (topo == null) {
                topo = searchAdminsForTopo(hostPortsArray, loginCreds);
            } 

            /* Still can't find a topology */
            if (topo == null) {
                throw topoLocEx;
            }
        }
        params = findParams(loginCreds);
    }

    /**
     * For callers who prefer to use Ping over TopologyLocator, because Ping
     * searches both RNs and Admin Services in search of a topology.
     * @throws KVStoreException if a topology can't be found.
     */
    public static Topology findTopology(String hostname, int port) 
        throws KVStoreException {
        List<String> helpers = new ArrayList<String>();
        helpers.add(new HostPort(hostname, port).toString());
        Ping ping = new Ping(helpers,
                             false, // verbose
                             false, // useJSON
                             System.err,
                             null);
        return ping.getTopology();
    }

    private Topology getTopology() {
        return topo;
    }

    /**
     * Used by PingCommand, invoked within the Admin CLI. A topology is
     * provided since it's already connected to an Admin, no need to find one.
     *
     * Providing a static convenience method reinforces the fact that since
     * each Ping instance accumulates state, each Ping instance should be
     * used for a single Ping call.
     */
    public static void pingTopology(Topology topo,
                                    Parameters params,
                                    boolean verbose,
                                    boolean getJson,
                                    PrintStream ps,
                                    LoginManager adminLoginManager) {
        Ping p = new Ping(topo, params, verbose, getJson, ps,
                          adminLoginManager);
        p.pingTopology();
    }


    /**
     * Ping all the SNs, RNs, and optionally admins, that make up the topology,
     * and print results to the specified PrintStream.  If params is non-null,
     * it will be used to discover admins, otherwise admins will not be
     * included.
     */
    public void pingTopology() {
        if (topo == null) {
            return;
        }

        /* Request status from each service in the store. */
        PingCollector collector =
            new PingCollector(topo, params, adminLoginManager);

        /*
         * Analyze the service status and generate an exist code. The exit code
         * acts as a summary of the findings, which indicates whether the
         * caller needs to take action.
         */
        exitCode = analyzeStatus(collector);

        /*
         * Create a JSON node that displays the ping results. The conversion to
         * json is not just a direct translation into a display format, it also
         * aggregates the information and creates overviews and summaries.
         */
        ObjectNode jsonTop;
        jsonTop = convertStatusToJson(collector);

        /* Using the JSON node as the source, display the results */
        if (getJson) {

            /*
             * Json mode only - display the exit code and any prescriptive
             * information. This is meant for scripting support, and therefore
             * is only available in json.
             *
             * Add information about the return code in the standard json
             * format for all CLIs.
             */
            createResultsJson(jsonTop, new PingResult(exitCode, null));

            /*
             * Add information about any problems found. This feature is
             * currently undocumented, only available via json, and is
             * currently governed by verbose.
             */
            if (verbose) {
               problemReport.addAll(collector.getProblems());
               addProblemReport(jsonTop);
            }

            final ObjectWriter writer = createWriter(true /* pretty */);
            try {
                ps.println(writer.writeValueAsString(jsonTop));
            } catch (IOException e) {
                ps.println(e);
            }
        } else {
            ps.print("Pinging components of ");
            ps.println(PingDisplay.displayTopologyOverview(jsonTop));
            ps.println(PingDisplay.displayShardOverview(jsonTop));
            final String adminOverview =
                PingDisplay.displayAdminOverview(jsonTop);
            if (!"".equals(adminOverview)) {
                ps.println(adminOverview);
            }
            for (JsonNode jsonZone : getArray(jsonTop, "zoneStatus")) {
                ps.println(PingDisplay.displayZoneOverview(jsonZone));
            }
            for (JsonNode jsonSN : getArray(jsonTop, "snStatus")) {
                ps.println(PingDisplay.displayStorageNode(jsonSN));
                final JsonNode jsonAdmin = jsonSN.get("adminStatus");
                if (jsonAdmin != null) {
                    ps.println(PingDisplay.displayAdmin(jsonAdmin));
                }
                for (JsonNode jsonRN : getArray(jsonSN, "rnStatus")) {
                    ps.println(PingDisplay.displayRepNode(jsonRN));
                }
            }
        }
    }

    /**
     * Displays the status values collected from the cluster components as
     * JSON. In some cases, status values are simply formatted and displayed,
     * in others, status values are aggregated or summarized.
     */
    private ObjectNode convertStatusToJson(PingCollector collector) {

        Map<StorageNode, StorageNodeStatus> snMap = collector.getSNMap();

        /* Group output by Storage Node */
        List<StorageNode> sns = new ArrayList<StorageNode>(snMap.keySet());
        Collections.sort(sns, new Comparator<StorageNode>() {
            @Override
            public int compare(StorageNode o1, StorageNode o2) {
                return o1.getStorageNodeId().getStorageNodeId() -
                    o2.getStorageNodeId().getStorageNodeId();
            }});

        /* Using the collected RN statuses, extract RN master status info */
        final Map<RepNode, RepNodeStatus> rnMap = collector.getRNMap();
        final Map<RepGroupId, RepNodeStatus> masterStatusMap =
            new HashMap<RepGroupId, RepNodeStatus>();
        for (Entry<RepNode, RepNodeStatus> e : rnMap.entrySet()) {
            final RepNodeStatus status = e.getValue();
            if ((status != null) && status.getReplicationState().isMaster()) {
                final RepNode rn = e.getKey();
                masterStatusMap.put(rn.getRepGroupId(), status);
            }
        }

        /*
         * Create a JSON object and construct the ping status display.
         * Start by summarizing topology
         */
        final ObjectNode jsonTop = createObjectNode();
        PingDisplay.topologyOverviewToJson(topo, jsonTop);

        /* Add admin overview to the display */
        final Map<AdminId, AdminInfo> adminMap = collector.getAdminMap();
        if (params != null) {
            final AdminStatusFunction adminStatusFunc =
                new AdminStatusFunction() {
                    @Override
                    public AdminStatus get(AdminId adminId) {
                        final AdminInfo adminInfo = adminMap.get(adminId);
                        return adminInfo.adminStatus;
                    }
                };
            PingDisplay.adminOverviewToJson(params, adminStatusFunc, jsonTop);
        }

        /*
         * Define a RN status function to extract info for shards and zones
         * overviews.
         */
        final RepNodeStatusFunction rnfunc = new RepNodeStatusFunction() {
            @Override
            public RepNodeStatus get(RepNode rn) {
                return rnMap.get(rn);
            }
        };

        /* Add a shard overview */
        PingDisplay.shardOverviewToJson(topo, rnfunc, jsonTop);

        /* Add zone overviews. */
        final ArrayNode jsonZones = jsonTop.putArray("zoneStatus");
        for (final Datacenter dc : topo.getSortedDatacenters()) {
            jsonZones.add(PingDisplay.zoneOverviewToJson(topo, dc, rnfunc));
        }

        /* Add SN, Admin, andRN status in SN order. */
        final ArrayNode jsonSNs = jsonTop.putArray("snStatus");
        for (StorageNode sn : sns) {
            StorageNodeStatus status = snMap.get(sn);
            final ObjectNode jsonSN = PingDisplay.storageNodeToJson(topo, sn,
                                                                  status);
            jsonSNs.add(jsonSN);
            for (Entry<AdminId, AdminInfo> aentry : adminMap.entrySet()) {
                final AdminInfo info = aentry.getValue();
                if ((info != null) &&
                    sn.getStorageNodeId().equals(info.snId)) {
                    jsonSN.put("adminStatus",
                               PingDisplay.adminToJson(aentry.getKey(),
                                                     info.adminStatus));
                    break;
                }
            }
            final ArrayNode jsonRNs = jsonSN.putArray("rnStatus");
            for (Entry<RepNode, RepNodeStatus> rentry : rnMap.entrySet()) {
                final RepNode rn = rentry.getKey();
                if (sn.getStorageNodeId().equals(rn.getStorageNodeId())) {
                    jsonRNs.add(PingDisplay.repNodeToJson
                                (rn,
                                 rentry.getValue(),
                                 masterStatusMap.get(rn.getRepGroupId()),
                                 null /* expectedStatus */));
                }
            }
        }
        return jsonTop;
    }

    /**
     * Using the topology to find SNs, find an AdminService to get some Params
     */
    private Parameters findParams(LoginCredentials loginCreds) {

        if (topo == null) {
            return null;
        }

        /* Look for admins to get parameters */
        for (StorageNode sn : topo.getStorageNodeMap().getAll()) {
            try {
                final CommandServiceAPI admin =
                    getAdmin(sn.getHostname(), sn.getRegistryPort(),
                             loginCreds);
                return admin.getParameters();
            } catch (NotBoundException ignore) {
                /* Ignore, there's no Admin on this SN */
            } catch (RemoteException e) {
                /*
                 * Note the problem - an Admin is registered on this SN, but
                 * it couldn't be accessed.
                 */
                problemReport.add
                    (new Problem(sn.getResourceId(),
                                 sn.getHostname(),
                                 sn.getRegistryPort(),
                                 "Admin Service exists on this SN but " +
                                  "ping couldn't contact it: ", e));
            }
        }

        /* Can't find any Admins, there should be some in the list. */
        problemReport.add(new Problem("Can't contact any Admin services " +
                                      "in the store"));
        return null;
    }

    /**
     * Given a set of SNs, find an AdminService to find a topology
     */
    private Topology searchAdminsForTopo(String[] hostPortStrings,
                                         LoginCredentials loginCreds) {

        final HostPort[] targetHPs = HostPort.parse(hostPortStrings);

        /* Look for admins to get parameters */
        for (HostPort hp : targetHPs) {
            try {
                final CommandServiceAPI admin =
                    getAdmin(hp.hostname(), hp.port(), loginCreds);
                return admin.getTopology();
            } catch (NotBoundException ignore) {
                /* Ignore, there's no Admin on this SN */
            } catch (RemoteException e) {
                /*
                 * Note the problem - an Admin is registered on this SN, but
                 * it couldn't be accessed.
                 */
                problemReport.add
                    (new Problem(hp.hostname(),
                                 hp.port(),
                                 "Admin Service exists on this SN but " +
                                  "ping couldn't contact it: ", e));
            } catch (Exception e) {
                /* Ignore, Admin failed to get topology when Admin is not
                 * configured or ready */
            }
        }

        /* Can't find any Admins, there should be some in the list. */
        problemReport.add(new Problem("Searching for topology, can't contact "+
                                      "any Admin services in the store"));
        return null;
    }

    private LoginManager getAdminLoginManager(
        String snHostname, int snRegistryPort,
        LoginCredentials loginCreds) {

        return (loginCreds != null) ?
            KVStoreLogin.getAdminLoginMgr(
                snHostname, snRegistryPort, loginCreds) :
            null;
    }

    /**
     * Get the CommandService on this particular SN.
     */
    private CommandServiceAPI getAdmin(String snHostname,
                                       int snRegistryPort,
                                       LoginCredentials loginCreds)
        throws NotBoundException, RemoteException {

        LoginManager snloginMgr = null;
        if (loginCreds != null) {
            snloginMgr = getAdminLoginManager(snHostname, snRegistryPort,
                                              loginCreds);
        }
        return RegistryUtils.getAdmin(snHostname, snRegistryPort, snloginMgr);
    }

    /**
     * Look at the status information collected from the components of the
     * store, and analyze it in the context of how the component fits within
     * the topology.
     *
     * Walk the topology, looking for matching entries in the service maps.
     * If a service is not found or not in an expected "good" state, make
     * note of it.  This isn't implemented to be efficient. It does a lot
     * of walking/scanning of structures.
     *
     * This function tracks:
     * - the total number of services not in a good state
     * - quorum for replicated services.
     * If a replication group (RN or admin) does not have quorum the global
     * noRNQuorum counter is incremented, indicating a situation that requires
     * immediate attention because the cluster may be partially inaccessible.
     */
    private ExitCode analyzeStatus(PingCollector collector) {
        if (topo == null) {
            return ExitCode.EXIT_TOPOLOGY_FAILURE;
        }

        final Set<AdminId> failedAdmins = new HashSet<AdminId>();
        final TreeMap<RepGroupId, Integer> failedShards =
            new TreeMap<RepGroupId, Integer>();

        int numFail = 0;

        /* Look for failed storageNodes */
        List<StorageNode> sns = topo.getSortedStorageNodes();
        final Map<StorageNode, StorageNodeStatus> snMap = collector.getSNMap();
        for (StorageNode sn : sns) {
            StorageNodeStatus snStatus = snMap.get(sn);
            ServiceStatus status = (snStatus == null ?
                                    ServiceStatus.UNREACHABLE :
                                    snStatus.getServiceStatus());
            if (!status.equals(ServiceStatus.RUNNING)) {
                problemReport.add(new Problem(sn.getResourceId(),
                                              sn.getHostname(),
                                              sn.getRegistryPort(),
                                              "Unexpected status " +
                                              status));
                numFail++;
            }
        }

        /* Does each shard have quorum? */
        final Map<RepNode, RepNodeStatus> rnMap = collector.getRNMap();
        final Map<ResourceId, ServiceChange> monitoredChanges =
                collector.getMonitoredChanges();
        int noRNQuorum = 0;
        for (RepGroup rg: topo.getRepGroupMap().getAll()) {

            /* Figure out what the quorum size is for this shard */
            Collection<RepNode> rns = rg.getRepNodes();
            int rf = rns.size();
            int numNeeded =  (rf/2 + 1);
            int numBadInShard = 0;

            for (RepNode rn: rns) {
                RepNodeStatus rnStatus = rnMap.get(rn);
                ServiceStatus status = (rnStatus == null ?
                                        ServiceStatus.UNREACHABLE :
                                        rnStatus.getServiceStatus());
                if (!status.equals(ServiceStatus.RUNNING)) {
                    RepNodeId rid = rn.getResourceId();
                    StringBuilder sb = new StringBuilder();
                    sb.append("RN is not running: ").append(status);

                    if (rnStatus == null) {
                        ServiceChange change = monitoredChanges.get(rid);
                        sb.append(", last known status is ");
                        if (change == null) {
                            sb.append("UNKNOWN");
                        } else {
                            String reportTime =
                                FormatUtils.formatDateAndTime
                                (change.getChangeTime());
                            sb.append(change.getStatus())
                                .append(", reported at ").append(reportTime);
                        }
                    }
                    StorageNode sn = topo.get(rn.getStorageNodeId());
                    problemReport.add(new Problem(rid,
                                                  sn.getHostname(),
                                                  sn.getRegistryPort(),
                                                  sb.toString()));
                    numFail++;
                    numBadInShard++;
                }
            }
            if (numBadInShard > 0) {
                failedShards.put(rg.getResourceId(), numBadInShard);
                if (numBadInShard >= numNeeded) {
                    noRNQuorum++;
                }
            }
        }

        boolean noAdminQuorum = false;
        if (params != null) {

            Map<AdminId, AdminInfo> adminMap = collector.getAdminMap();
            int numAdmins = params.getAdminIds().size();
            int numNeeded = (numAdmins/2 + 1);
            for (final AdminId aid : params.getAdminIds()) {
                AdminInfo ainfo = adminMap.get(aid);
                AdminStatus adminStatus = null;
                if (ainfo != null) {
                    adminStatus = ainfo.adminStatus;
                }
                ServiceStatus status = (adminStatus == null ?
                                        ServiceStatus.UNREACHABLE :
                                        adminStatus.getServiceStatus());
                if (!status.equals(ServiceStatus.RUNNING)) {
                    /* Find the host/port for this Admin. */
                    AdminParams ap = params.get(aid);
                    StorageNodeId snId = ap.getStorageNodeId();
                    StorageNode sn = topo.get(snId);
                    problemReport.add(new Problem(aid,
                                                  sn.getHostname(),
                                                  sn.getRegistryPort(),
                                                 "Admin is not running: " +
                                                  status));
                    failedAdmins.add(aid);
                    numFail++;
                }
            }
            if (failedAdmins.size() >= numNeeded) {
                noAdminQuorum = true;
            }
        } else {
            /*
             * Cannot establish the health of the admins.  This is a failure.
             */
            numFail++;
        }

        /* Use the generated information to figure out an exit code. */
        if (noRNQuorum > 0) {
            return ExitCode.EXIT_NO_SHARD_QUORUM;
        } else if (noAdminQuorum) {
            return ExitCode.EXIT_NO_ADMIN_QUORUM;
        } else if (numFail > 0) {
            return ExitCode.EXIT_OPERATIONAL;
        } else {
            return ExitCode.EXIT_OK;
        }
    }

    /* --------- Json parsing, display support ---------- */
    /**
     * TODO: refactor this with CommandJsonUtils.updateNodeWithResult when we
     * unify the admin automation work with scripting support for SNA commands
     * and non-admin utilities like ping.
     */
    private static void createResultsJson(ObjectNode on, PingResult result) {
        if (result == null) {
            return;
        }
        on.put(CommandJsonUtils.FIELD_OPERATION, "ping");
        on.put(CommandJsonUtils.FIELD_RETURN_CODE, result.getErrorCode());
        on.put(CommandJsonUtils.FIELD_DESCRIPTION, result.getDescription());
        on.put(EXIT_CODE_FIELD, result.getExitCode().value());
        return;
    }

    private static void displayExitJson(PrintStream ps, 
                                        ExitCode exitCode,
                                        String errorMsg) {
        /* Package up the exit results in a json node */
        final ObjectNode exitStatus = createObjectNode();
        PingResult pingResult = new PingResult(exitCode, errorMsg);
        createResultsJson(exitStatus, pingResult);

        /* print the json node. */
        final ObjectWriter writer = createWriter(true /* pretty */);
        try {
            ps.println(writer.writeValueAsString(exitStatus));
        } catch (IOException e) {
            ps.println(e);
        }
    }

    /**
     * Create a json array containing each problem. Currently unadvertised.
     * "problems" : [ {
     *    "component" : null,
     *    "hostname" : null,
     *    "port" : 0,
     *    "description" : "Can't contact any Admin services in the store"
     *  }, {
     *    "component" : "sn1",
     *    "hostname" : "localhost",
     *     "port" : 5001,
     *     "description" : "Unexpected status UNREACHABLE"
     *  }, {
     *    "component" : "sn1",
     *    "hostname" : "localhost",
     *    "port" : 5001,
     *    "description" : "No RMI service for SN kvtest-oracle.kv.util.PingTest-testPing:sn1:MAIN"
     *  } ]
     */
    private void addProblemReport(ObjectNode resultsJson) {

        if (problemReport.isEmpty()) {
            return;
        }

        /*
         * Sort the display of problems; there may be more than one per
         * component.
         */
        Problem[] problemArray = new Problem[problemReport.size()];
        problemArray = problemReport.toArray(problemArray);
        Arrays.sort(problemArray);

        ArrayNode problemJson = resultsJson.putArray("problems");
        for (Problem p : problemArray) {
            p.addToArrayNode(problemJson);
        }
    }

    private static class PingResult implements CommandResult {

        private final ExitCode exitCode;
        private final String errorMsg;

        PingResult(ExitCode exitCode, String errorMsg) {
            this.exitCode = exitCode;
            this.errorMsg = errorMsg;
        }

        @Override
        public String getReturnValue() {
            /* No values are returned by this command */
            return null;
        }

        ExitCode getExitCode() {
            return exitCode;
        }

        @Override
        public String getDescription() {
            if (errorMsg == null) {
                return exitCode.getDescription();
            } 
            return exitCode.getDescription() + " - " + errorMsg;
        }

        @Override
        public int getErrorCode() {
            return exitCode.getErrorCode().getValue();
        }

        @Override
        public String[] getCleanupJobs() {
            return null;
        }
    }

    /**
     * This is purely a backward compatibility method to supply the same
     * output for a running but not-deployed SNA that currently exists.
     * Replace and remove this method when the SNA status command has been
     * implemented and becomes the advised way to check on an individual
     * SNA's status.
     */
    private static void checkIfSNAIsDeployed(String hostPort) {
        HostPort hp = HostPort.parse(hostPort);
        Registry snRegistry;
        try {
            snRegistry = RegistryUtils.getRegistry(hp.hostname(), hp.port(),
                                      null /* storeName */);

            final List<String> serviceNames = new ArrayList<String>();
            Collections.addAll(serviceNames, snRegistry.list());
            if (serviceNames.contains(GlobalParams.SNA_SERVICE_NAME)) {
                /* not yet registered. */
                System.err.println
                    ("SNA at hostname: " + hp.hostname() +
                     ", registry port: " + hp.port() +
                     " is not registered." +
                     "\n\tNo further information is available");
                return;
            }
        } catch (RemoteException e) {
            System.err.println("Could not connect to registry at " + hostPort +
                               " " + e.getMessage());
        }
    }

    /**
     * Exit the process with the appropriate exit code, generating the
     * appropriate message.
     * @param dontExit if false, don't exit if ping is being called from a
     * Junit unit test, as that will kill the test.
     */
    private static void exit(boolean dontExit,
                             String msg,
                             ExitCode exitCode,
                             boolean getJson,
                             PrintStream ps) {
        if ((msg != null) && (ps != null)) {
            if (getJson) {
                displayExitJson(ps, exitCode, msg);
            } else {
                ps.println(msg);
            }
        }

        if (!dontExit) {
            System.exit(exitCode.value());
        }
    }

    /**
     * Exit, but no need to generate an exit message.
     */
    private static void exitNoDisplay(boolean dontExit,
                                      ExitCode exitCode) {
        exit(dontExit, null, exitCode, false, null);
    }

    /* A class to encapsulate problem reports, keyed by a component id. */
    static class Problem implements Comparable<Problem> {
        private final String componentName;
        private final String description;
        private final String hostname;
        private final int port;

        Problem(ResourceId resourceId,
                String hostname,
                int port,
                String description) {
            this.componentName = resourceId.toString();
            this.description = description;
            this.hostname = hostname;
            this.port = port;
        }

        Problem(ResourceId resourceId,
                String hostname,
                int port,
                String description,
                Exception e) {
            this.componentName = resourceId.toString();
            this.description = description + " " + e.getMessage();
            this.hostname = hostname;
            this.port = port;
        }

        Problem(String description) {
            this.componentName = null;
            this.description = description;
            this.hostname = null;
            this.port = 0;
        }


        public Problem(String hostname, int port, String description, 
                       RemoteException e) {
            this.componentName = null;
            this.description = description + " " + e.getMessage();
            this.hostname = hostname;
            this.port = port;
        }

        private void addToArrayNode(ArrayNode problemList) {
            final ObjectNode on = problemList.addObject();
            on.put("component", componentName);
            on.put("hostname", hostname);
            on.put("port", port);
            on.put("description", description);
        }

        /** Order by component name */
        @Override
        public int compareTo(Problem o) {
            if (o == null) {
                return 1;
            }

            if (o.componentName == null) {
                return 1;
            }

            if (componentName == null) {
                if (o.componentName == null) {
                    return 0;
                }
                return -1;
            }
            return componentName.compareTo(o.componentName);
        }
    }
}
