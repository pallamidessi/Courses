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

package oracle.kv.impl.rep;

import java.util.HashMap;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.security.auth.Subject;

import oracle.kv.LoginCredentials;
import oracle.kv.PasswordCredentials;
import oracle.kv.impl.admin.param.GlobalParams;
import oracle.kv.impl.admin.param.RepNodeParams;
import oracle.kv.impl.admin.param.SecurityParams;
import oracle.kv.impl.admin.param.StorageNodeParams;
import oracle.kv.impl.api.RequestDispatcher;
import oracle.kv.impl.api.TopologyManager;
import oracle.kv.impl.param.ParameterMap;
import oracle.kv.impl.rep.RepNodeService.KVStoreCreator;
import oracle.kv.impl.rep.login.FailoverSessionManager;
import oracle.kv.impl.rep.login.KVSessionManager;
import oracle.kv.impl.security.AccessChecker;
import oracle.kv.impl.security.AccessCheckerImpl;
import oracle.kv.impl.security.Authenticator;
import oracle.kv.impl.security.AuthenticatorManager;
import oracle.kv.impl.security.AuthenticatorManager.SystemAuthMethod;
import oracle.kv.impl.security.ExecutionContext;
import oracle.kv.impl.security.KVBuiltInRoleResolver;
import oracle.kv.impl.security.KVStoreUserPrincipal;
import oracle.kv.impl.security.PasswordAuthenticator;
import oracle.kv.impl.security.ProxyCredentials;
import oracle.kv.impl.security.RoleInstance;
import oracle.kv.impl.security.RoleResolver;
import oracle.kv.impl.security.SignatureHelper;
import oracle.kv.impl.security.TopoSignatureHelper;
import oracle.kv.impl.security.UserVerifier;
import oracle.kv.impl.security.login.InternalLoginManager;
import oracle.kv.impl.security.login.KerberosInternalCredentials;
import oracle.kv.impl.security.login.LoginManager;
import oracle.kv.impl.security.login.LoginUpdater.GlobalParamsUpdater;
import oracle.kv.impl.security.login.LoginUpdater.ServiceParamsUpdater;
import oracle.kv.impl.security.login.TokenResolverImpl;
import oracle.kv.impl.security.login.TokenVerifier;
import oracle.kv.impl.security.login.TopoTopoResolver;
import oracle.kv.impl.security.login.TopoTopoResolver.TopoMgrTopoHandle;
import oracle.kv.impl.security.login.TopologyResolver.SNInfo;
import oracle.kv.impl.security.login.UserLoginCallbackHandler;
import oracle.kv.impl.security.login.UserLoginHandler;
import oracle.kv.impl.security.metadata.KVStoreUser;
import oracle.kv.impl.security.metadata.SecurityMDChange;
import oracle.kv.impl.security.metadata.SecurityMDUpdater.RoleChangeUpdater;
import oracle.kv.impl.security.metadata.SecurityMDUpdater.UserChangeUpdater;
import oracle.kv.impl.security.metadata.SecurityMetadata;
import oracle.kv.impl.security.util.CacheBuilder.CacheConfig;
import oracle.kv.impl.topo.Topology;

/**
 * This is the security management portion of the RepNode. It constructs and
 * houses the AccessCheck implementation, etc.
 */
public class RepNodeSecurity implements GlobalParamsUpdater,
                                        ServiceParamsUpdater,
                                        UserChangeUpdater,
                                        RoleChangeUpdater {

    private final RepNodeService repNodeService;
    private final AccessCheckerImpl accessChecker;
    private final TokenResolverImpl tokenResolver;
    private final TopoMgrTopoHandle topoMgrHandle;
    private final TopoTopoResolver topoResolver;
    private final InternalLoginManager loginMgr;
    private final String storeName;
    private final TokenVerifier tokenVerifier;
    private final Logger logger;
    private final UserVerifier userVerifier;
    private final SignatureHelper<Topology> topoSignatureHelper;
    private KVSessionManager kvSessionManager;
    private final Map<String, Authenticator> authenticators;

    /*
     * Size of subject-privilege cache in access checker
     */
    private static final int CHECKER_SUBJECT_CACHE_SIZE = 100;

    /**
     * Constructor
     */
    public RepNodeSecurity(RepNodeService rnService, Logger logger) {
        this.repNodeService = rnService;
        this.logger = logger;
        final RepNodeService.Params params = rnService.getParams();
        final SecurityParams secParams = params.getSecurityParams();
        this.storeName = params.getGlobalParams().getKVStoreName();
        this.kvSessionManager = null;

        if (secParams.isSecure()) {
            this.userVerifier = new RepNodeUserVerifier();
            final StorageNodeParams snParams = params.getStorageNodeParams();
            final String hostname = snParams.getHostname();
            final int registryPort = snParams.getRegistryPort();

            this.topoMgrHandle = new TopoMgrTopoHandle(null);
            final SNInfo localSNInfo = new SNInfo(hostname, registryPort,
                                                  rnService.getStorageNodeId());
            this.topoResolver = new TopoTopoResolver(topoMgrHandle,
                                                     localSNInfo, logger);
            this.loginMgr = new InternalLoginManager(topoResolver);
            this.tokenResolver =
                new TokenResolverImpl(hostname, registryPort, storeName,
                                      topoResolver, loginMgr, logger);

            final RepNodeParams rp = rnService.getRepNodeParams();
            final int tokenCacheCapacity = rp.getLoginCacheSize();

            final GlobalParams gp = rnService.getParams().getGlobalParams();
            final long tokenCacheEntryLifetime =
                gp.getLoginCacheTimeoutUnit().toMillis(
                    gp.getLoginCacheTimeout());
            final CacheConfig tokenCacheConfig =
                    new CacheConfig().capacity(tokenCacheCapacity).
                                      entryLifetime(tokenCacheEntryLifetime);
            tokenVerifier =
                new TokenVerifier(tokenCacheConfig, tokenResolver);

            /*
             * On RepNode, since the execution context will be constructs many
             * times due to kv operation call, we set the subject-privileges
             * cache timeout value as the same of session lifetime.
             */
            final long subjCacheTimeout = gp.getSessionTimeout();
            final CacheConfig subjectCacheConfig =
                new CacheConfig().capacity(CHECKER_SUBJECT_CACHE_SIZE).
                                  entryLifetime(subjCacheTimeout);
            this.accessChecker =
                new AccessCheckerImpl(tokenVerifier,
                                      new RepNodeRoleResolver(),
                                      subjectCacheConfig,
                                      logger);
            this.topoSignatureHelper =
                TopoSignatureHelper.buildFromSecurityParams(secParams);
            this.authenticators = new HashMap<>();

            /* Initialize all supported system authenticators */
            for (final SystemAuthMethod sysAuth : SystemAuthMethod.values()) {
                final Authenticator authenticator =
                    createAuthenticator(sysAuth, secParams);

                if (authenticator != null) {
                    logger.info("RNSecurity: " + sysAuth +
                                " authenticator is initialized");
                    authenticators.put(sysAuth.name(), authenticator);
                }
            }
        } else {
            this.userVerifier = null;
            this.accessChecker = null;
            this.tokenResolver = null;
            this.topoMgrHandle = null;
            this.topoResolver = null;
            this.loginMgr = null;
            this.tokenVerifier = null;
            this.topoSignatureHelper = null;
            this.authenticators = null;
        }
    }

    /**
     * Updates the security configuration to use the request dispatcher.
     * There is an inter-dependency between these two classes, so we delay
     * the use of the dispatcher until both instances are available.
     */
    void setDispatcher(RequestDispatcher dispatcher) {
        if (tokenResolver != null) {
            this.kvSessionManager =
                new KVSessionManager(dispatcher,
                                     repNodeService.getRepNodeParams(),
                                     FailoverSessionManager.PERSISTENT_PREFIX,
                                     UserLoginHandler.SESSION_ID_RANDOM_BYTES,
                                     userVerifier,
                                     logger);
            tokenResolver.setPersistentResolver(kvSessionManager);
        }
    }

    /**
     * Enable the security functions.  This is dependent on topology startup.
     */
    void startup(KVStoreCreator creator) {
        if (kvSessionManager != null) {
            kvSessionManager.start(creator);
        }
    }

    /**
     * Stop the use of persistent store by security functions.
     */
    public void stop() {
        if (kvSessionManager != null) {
            kvSessionManager.stop();
        }
    }

    public AccessChecker getAccessChecker() {
        return accessChecker;
    }

    public LoginManager getLoginManager() {
        return loginMgr;
    }

    public KVSessionManager getKVSessionManager() {
        return kvSessionManager;
    }

    public UserVerifier getUserVerifier() {
        return userVerifier;
    }

    /**
     * Returns the topology signature helper.  Null if security is not enabled.
     */
    public SignatureHelper<Topology> getTopoSignatureHelper() {
        return topoSignatureHelper;
    }

    void setTopologyManager(TopologyManager topoMgr) {
        if (topoMgrHandle != null) {
            topoMgrHandle.setTopoMgr(topoMgr);
        }
    }

    private KVStoreUser loadUserFromMd(String userName) {
        final SecurityMetadata secMd = repNodeService.getSecurityMetadata();
        if (secMd == null) {
            logger.info(
                "Unable to verify user credentials with no security " +
                "metadata available");
           return null;
        }
        return secMd.getUser(userName);
    }

    private Subject makeUserSubject(String userName) {
        final SecurityMetadata secMd = repNodeService.getSecurityMetadata();
        if (secMd == null) {
            logger.info("Unable to make user subject with " +
                        "no security metadata available");
            return null;
        }
        final KVStoreUser user = secMd.getUser(userName);

        if (user == null || !user.isEnabled()) {
            logger.log(Level.INFO, "User " + userName + " is not valid");
            return null;
        }
        return user.makeKVSubject();
    }

    @Override
    public void newServiceParameters(ParameterMap map) {
        if (tokenVerifier == null) {
            return;
        }
        final RepNodeParams rp = new RepNodeParams(map);
        final int newCapacity = rp.getLoginCacheSize();

        /* Update the loginCacheSize if a new value is specified */
        if (tokenVerifier.updateLoginCacheSize(newCapacity)) {
            logger.info(String.format(
                "RNSecurity: loginCacheSize has been updated to %d",
                newCapacity));
        }
    }

    @Override
    public void newGlobalParameters(ParameterMap map) {
        if (tokenVerifier == null) {
            return;
        }

        final GlobalParams gp = new GlobalParams(map);
        final long newLifeTime =
            gp.getLoginCacheTimeoutUnit().toMillis(gp.getLoginCacheTimeout());

        /* Update the loginCacheTimeout if a new value is specified */
        if (tokenVerifier.updateLoginCacheTimeout(newLifeTime)) {
            logger.info(String.format(
                "RNSecurity: loginCacheTimeout has been updated to %d ms",
                newLifeTime));
        }

        if (authenticators == null) {
            return;
        }
        final String[] enabledAuthMethods = gp.getUserExternalAuthMethods();

        for (Map.Entry<String, Authenticator> entry :
                 authenticators.entrySet()) {
            final String authenName = entry.getKey();
            boolean enabled = false;
            for (String authMethod : enabledAuthMethods) {
                if (authenName.equals(authMethod)) {
                    enabled = true;
                    break;
                }
            }
            if (!enabled) {
                final Authenticator authen = entry.getValue();
                logger.info("RNSecurity: disable authenticator " + authenName);
                authen.resetAuthenticator();
            }
        }
    }

    private class RepNodeUserVerifier implements UserVerifier {

        /* Default authenticator - password based */
        private RepNodePasswordAuthenticator defaultAuthenticator;

        RepNodeUserVerifier() {
            this.defaultAuthenticator = new RepNodePasswordAuthenticator();
        }

        /**
         * Verify that the login credentials are valid and return a subject that
         * identifies the user.  If ProxyCredentials are passed, no validation
         * is performed, so the caller must limit use of ProxyCredentials to
         * INTERNAL roles.
         */
        @Override
        public Subject verifyUser(LoginCredentials creds,
                                  UserLoginCallbackHandler handler) {

            if (defaultAuthenticator.authenticate(creds, handler)) {
                return makeUserSubject(creds.getUsername());
            }

            if (repNodeService == null) {
                return null;
            }
            final GlobalParams gp = repNodeService.getParams().getGlobalParams();
            final String[] authMethods = gp.getUserExternalAuthMethods();
            for (String authMethod : authMethods) {
                final Authenticator authen = authenticators.get(authMethod);

                if (authen != null && authen.authenticate(creds, handler)) {
                    return makeUserSubject(creds.getUsername());
                }
            }

            if (creds instanceof ProxyCredentials) {
                return makeUserSubject(creds.getUsername());
            }

            /* Check specified login credentials is valid */
            if (!(creds instanceof ProxyCredentials) &&
                !(creds instanceof PasswordCredentials) &&
                !(creds instanceof KerberosInternalCredentials)) {
                logger.info(
                    "Encountered unsupported login credentials of type " +
                    creds.getClass());
            }
            return null;
        }

        /**
         * Verify that the Subject is valid.
         */
        @Override
        public Subject verifyUser(Subject subj) {

            final KVStoreUserPrincipal userPrinc =
                ExecutionContext.getSubjectUserPrincipal(subj);
            if (userPrinc == null) {
                return null;
            }

            final SecurityMetadata secMd = repNodeService.getSecurityMetadata();
            if (secMd == null) {
                logger.info(
                    "Unable to verify user with no security metadata " +
                    "available");
                return null;
            }

            final KVStoreUser user = secMd.getUser(userPrinc.getName());

            if (user == null || !user.isEnabled()) {
                logger.info(
                    "User " + userPrinc.getName() + " is not valid");
                return null;
            }

            return subj;
        }
    }

    /**
     * A role resolver resides on RepNode, in which the role name is tried to
     * resolve as system built-in roles first, and then the user-defined role.
     */
    private class RepNodeRoleResolver implements RoleResolver {

        @Override
        public RoleInstance resolve(String roleName) {
            RoleInstance roleInstance =
                KVBuiltInRoleResolver.resolveRole(roleName);

            if (roleInstance == null) {
                final SecurityMetadata secMd =
                    repNodeService.getSecurityMetadata();

                if (secMd == null) {
                    return null;
                }
                roleInstance = secMd.getRole(roleName);
            }
            return roleInstance;
        }
    }

    private class RepNodePasswordAuthenticator extends PasswordAuthenticator {

        @Override
        public KVStoreUser loadUserFromStore(String userName) {
            return loadUserFromMd(userName);
        }

        @Override
        public void logMessage(Level level, String msg) {
            logger.log(level, msg);
        }
    }

    @Override
    public void newUserDefinition(SecurityMDChange mdChange) {
        /* A guard */
        if (!(mdChange.getElement() instanceof KVStoreUser)) {
            throw new AssertionError();
        }
        final KVStoreUser user = (KVStoreUser)mdChange.getElement();

        if (tokenVerifier == null) {
            return;
        }
        if (tokenVerifier.updateLoginCacheSessions(user)) {
            logger.info(String.format(
                "RNSecurity: update sessions in login cache with " +
                "metadata %d", mdChange.getSeqNum()));
        }
        if (accessChecker.updateUserDefinition(user)) {
            logger.fine(String.format("RNSecurity: updated user %s " +
                "definition in access checker privilege cache",
                user.getName()));
        }
    }

    @Override
    public void newRoleDefinition(SecurityMDChange mdChange) {
        /* A guard */
        if (!(mdChange.getElement() instanceof RoleInstance)) {
            throw new AssertionError();
        }
        final RoleInstance role = (RoleInstance) mdChange.getElement();

        if (accessChecker == null) {
            return;
        }
        if (accessChecker.updateRoleDefinition(role)) {
            logger.fine(String.format("RNSecurity: update role %s " +
                "definition in access checker privilege cache", role.name()));
        }
    }

    private Authenticator createAuthenticator(SystemAuthMethod authMethod,
                                              SecurityParams secParams) {
        try {
            return AuthenticatorManager.getAuthenticator(authMethod.name(),
                                                         secParams);
        } catch (Exception e) {
            logger.warning(String.format("RNSecurity: initialize " +
                "authenticator " + authMethod + " error " + e.getMessage()));
            return null;
        }
    }
}
