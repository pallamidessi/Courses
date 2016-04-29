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

import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import oracle.kv.impl.fault.DatabaseNotReadyException;
import oracle.kv.impl.fault.RNUnavailableException;
import oracle.kv.impl.metadata.Metadata;
import oracle.kv.impl.metadata.Metadata.MetadataType;
import oracle.kv.impl.security.metadata.SecurityMDChange;
import oracle.kv.impl.security.metadata.SecurityMDListener;
import oracle.kv.impl.security.metadata.SecurityMetadata;
import oracle.kv.impl.security.metadata.SecurityMetadataInfo;
import oracle.kv.impl.security.metadata.UserRoleUpdater;

import com.sleepycat.je.rep.ReplicatedEnvironment;

/**
 * All access to the in-memory copy of security metadata on RepNode is
 * coordinated through this class.
 * <p>
 * Note that the RepNode does NOT build a new SecurityMetadata instance when it
 * starts. The Admin will push its security metadata to the RepNode when it is
 * created. The push will be performed through the update() of this class.
 * <p>
 * The persistence of the in-memory security metadata copy will be performed
 * whenever updated. The security metadata will be stored in the DB of RepNode
 * via the EntityStore on the RepNode's environment.
 * <p>
 * This class may also need to provide a series of update listeners, which will
 * be used in session state updating caused by any security definition changes.
 */
public class SecurityMetadataManager extends MetadataManager<SecurityMetadata> {

    /* Default value of maximum length of changes maintained. */
    private static final int MAX_CHANGES = 1000;

    /* In-memory copy of security metadata */
    private volatile SecurityMetadata securityMetadata;

    private final String kvstoreName;

    /* The max number of changes retained in the security metadata */
    private final int maxChanges;

    /* Track user role changes in security metadata */
    private final UserRoleUpdater userRoleTracker =
        new UserRoleUpdater();

    public SecurityMetadataManager(final RepNode repNode,
                                   final String storeName,
                                   final Logger logger) {
        this(repNode, storeName, MAX_CHANGES, logger);
    }

    public SecurityMetadataManager(final RepNode repNode,
                                   final String storeName,
                                   final int maxSecMDChanges,
                                   final Logger logger) {
        super(repNode, logger);

        if (maxSecMDChanges <= 0) {
            throw new IllegalArgumentException(
                "Max change limit must be a positive integer.");
        }
        this.kvstoreName = storeName;
        this.maxChanges = maxSecMDChanges;
    }

    @Override
    protected MetadataType getType() {
        return MetadataType.SECURITY;
    }

    /**
     * Adds a MD change update listener to help track SecurityMD changes.
     *
     * @param listener the new listener
     */
    void addUpdateListener(SecurityMDListener listener) {
        userRoleTracker.addListener(listener);
    }

    /**
     * Notify listeners security metadata changes.
     *
     * @param oldMd security metadata memory copy before changed.
     * @param mdChanges security metadata changes.
     */
    private void invokeListeners(SecurityMetadata oldMd,
                                 List<SecurityMDChange> mdChanges){

        /* 
         * Pass old metadata memory copy to UserRoleTracker for resolving and
         * filtering metadata changes.
         */
        userRoleTracker.notifyListeners(oldMd, mdChanges);
    }

    /**
     * Updates the security metadata by replacing the entire copy with a new
     * instance. This is typically done in response to a request from the Admin.
     * Or if the security metadata cannot be updated incrementally because the
     * necessary sequence of changes is not available in incremental form.
     * <p>
     * The update is only done if the security metadata is not current. If the
     * security metadata needs to be updated, but the update failed false is
     * returned. Otherwise true is returned. Note that a null security metadata
     * has zero as its sequence number by default.
     *
     * @param newSecurityMD the new security metadata copy
     *
     * @return false if the update failed
     */
    public synchronized boolean update(final SecurityMetadata newSecurityMD) {
        if (newSecurityMD == null) {
            throw new NullPointerException(
                "The new copy of SecurityMetadata should not be null.");
        }

        if (!kvstoreName.equals(newSecurityMD.getKVStoreName())) {
            throw new IllegalStateException(
                "Trying to update with security metadata from store: " +
                newSecurityMD.getKVStoreName() + ", but expected: " +
                kvstoreName);
        }

        final ReplicatedEnvironment repEnv = repNode.getEnv(1);
        if ((repEnv == null) || !repEnv.getState().isMaster()) {
            return false;
        }
        securityMetadata = getSecurityMetadata();

        final int currentSeqNum = (securityMetadata == null) ?
                                  Metadata.EMPTY_SEQUENCE_NUMBER :
                                  securityMetadata.getSequenceNumber();

        final int newSeqNum = newSecurityMD.getSequenceNumber();

        if (currentSeqNum >= newSeqNum) {
            logger.log(Level.INFO, "Security metadata update skipped. " +
                       "Current seq #: {0} Update seq #: {1}",
                       new Object[]{currentSeqNum, newSeqNum});
            return true;
        }

        /*
         * Prune the new security metadata changes to make it under the limit
         * of maximum changes.
         */
        final SecurityMetadata prunedMd = pruneChanges(
            newSecurityMD, maxChanges, Integer.MAX_VALUE);

        /* In case previous changes have been discarded */
        final int startSeqNum = Math.max(
            currentSeqNum, prunedMd.getFirstChangeSeqNum());

        if (persistMetadata(prunedMd)) {
            invokeListeners(securityMetadata, prunedMd.getChanges(startSeqNum));
            securityMetadata = prunedMd;
            logger.log(Level.INFO,
                       "Security metadata updated from seq#: {0} to {1}",
                       new Object[] { currentSeqNum, newSeqNum });
        }
        return true;
    }

    /**
     * Performs an incremental update to the security metadata.
     * <p>
     * An update may result in the security metadata changes being pruned so
     * that only the configured number of changes are retained.
     * <p>
     * 
     * @param newSecurityMDInfo the SecurityMetadataInfo containing the changes
     * to be made
     *
     * @return the sequence number of the security metadata resulting from the
     * operation. If the input metadata contains no changes or if it has a
     * base sequence number that is greater than the current security
     * metadata sequence number then no change to the metadata is made.
     */
    public synchronized int
        update(final SecurityMetadataInfo newSecurityMDInfo) {

        final ReplicatedEnvironment repEnv = repNode.getEnv(1);
        if (repEnv == null) {
            return Metadata.EMPTY_SEQUENCE_NUMBER;
        }
        securityMetadata = getSecurityMetadata();
        if (!repEnv.getState().isMaster()) {
            return securityMetadata.getSequenceNumber();
        }

        final int prevSeqNum = (securityMetadata == null) ?
                                Metadata.EMPTY_SEQUENCE_NUMBER :
                                securityMetadata.getSequenceNumber();

        if (newSecurityMDInfo.getChanges() == null ||
            newSecurityMDInfo.getChanges().isEmpty()) {
            /* Unexpected, should not happen. */
            logger.warning(
                "Empty change list sent for security metadata update");
            return prevSeqNum;
        }

        int startSeqNum =
            newSecurityMDInfo.getChanges().get(0).getSeqNum();
        if (startSeqNum > (prevSeqNum + 1)) {
            logger.info("Ignoring security metadata update request. " +
                        "Current seq num: " + prevSeqNum + " first change: " +
                        newSecurityMDInfo.getChanges().get(0).getSeqNum());
            return prevSeqNum;
        }

        final SecurityMetadata secMDCopy =
                (securityMetadata == null) ?
                new SecurityMetadata(
                    kvstoreName, newSecurityMDInfo.getSecurityMetadataId()) :
                securityMetadata.getCopy();

        if (!secMDCopy.apply(newSecurityMDInfo.getChanges())) {
            /* Not changed */
            return prevSeqNum;
        }
        final SecurityMetadata prunedMd = pruneChanges(
            secMDCopy, maxChanges, startSeqNum);

        /* In case previous changes have been discarded */
        startSeqNum = Math.max(startSeqNum, prunedMd.getFirstChangeSeqNum());
        if (persistMetadata(prunedMd)) {
            invokeListeners(securityMetadata, prunedMd.getChanges(startSeqNum));
            securityMetadata = prunedMd;
            logger.log(Level.INFO,
                       "Security metadata updated from seq#: {0} to {1}",
                       new Object[] { prevSeqNum,
                                      securityMetadata.getSequenceNumber() });
        }
        return securityMetadata.getSequenceNumber();
    }

    /**
     * Forces a refresh of the security metadata due to it being updated by
     * the master. Called from the database trigger.
     */
    @Override
    protected synchronized void update(ReplicatedEnvironment repEnv) {
        if (securityMetadata == null) {
            return;
        }
        final SecurityMetadata oldMd = securityMetadata.getCopy();
        int startSeqNum = oldMd.getSequenceNumber();
        securityMetadata = null;
        getSecurityMetadata();

        /* In case previous changes have been discarded */
        startSeqNum = Math.max(
            startSeqNum, securityMetadata.getFirstChangeSeqNum());
        invokeListeners(oldMd, securityMetadata.getChanges(startSeqNum));
    }

    public synchronized SecurityMetadata getSecurityMetadata() {
        if (securityMetadata == null) {
            try {
                securityMetadata = fetchMetadata();
            } catch (DatabaseNotReadyException dnre) {
                /* Throwing RNUnavailableException should cause a retry */
                throw new RNUnavailableException(
                    "Security metadata database is not opened yet.");
            }
        }
        return securityMetadata;
    }

    /**
     * Prune the changes stored in a security metadata, making the length of
     * the change list no larger than the predefined maxChange limit. This is
     * intended for putting a limit on the memory usage of the security metadata
     * copy.
     *
     * @param secMD the security metadata to prune
     * @param limit the limit on the length of change list
     * @param minRetainSeqNum the minimum seqNum of changes need to be retained
     * @return the security metadata after pruning
     */
    private static SecurityMetadata pruneChanges(final SecurityMetadata secMD,
                                                 final int limit,
                                                 final int minRetainSeqNum) {
        final int firstChangeSeqNum = secMD.getFirstChangeSeqNum();

        if (firstChangeSeqNum < 0) {
            /* No change to prune */
            return secMD;
        }

        final int newStartSeqNum = Math.min(
            secMD.getSequenceNumber() - limit + 1, minRetainSeqNum);
        if (newStartSeqNum > firstChangeSeqNum) {
            secMD.discardChanges(newStartSeqNum);
        }
        return secMD;
    }
}
