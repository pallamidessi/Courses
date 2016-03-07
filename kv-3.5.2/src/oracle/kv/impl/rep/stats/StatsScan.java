/*-
 *
 *  This file is part of Oracle NoSQL Database
 *  Copyright (C) 2011, 2014 Oracle and/or its affiliates.  All rights reserved.
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

package oracle.kv.impl.rep.stats;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import oracle.kv.Durability;
import oracle.kv.DurabilityException;
import oracle.kv.FaultException;
import oracle.kv.RequestTimeoutException;
import oracle.kv.Version;
import oracle.kv.impl.api.table.TableMetadata;
import oracle.kv.impl.metadata.Metadata.MetadataType;
import oracle.kv.impl.rep.RepNode;
import oracle.kv.impl.rep.stats.StatsLeaseManager.LeaseInfo;
import oracle.kv.table.Row;
import oracle.kv.table.TableAPI;
import oracle.kv.table.WriteOptions;

import com.sleepycat.je.CursorConfig;
import com.sleepycat.je.Database;
import com.sleepycat.je.Environment;
import com.sleepycat.je.TransactionConfig;
import com.sleepycat.je.rep.ReplicatedEnvironment;

/**
 * the class collects statistics information into statistics tables. The
 * subclasses of StatsScan provide the partition or shard-index specific
 * behaviors.
 */
@SuppressWarnings("deprecation")
public abstract class StatsScan <T extends LeaseInfo> {

    /* it is the RN which KeyStatsCollector resides */
    protected final RepNode repNode;

    /* Used to store the statistics results into statistics tables */
    private final TableAPI tableAPI;

    /* Logger handler */
    protected final Logger logger;

    /* The batch size to be used when iterating over keys. */
    protected final static int BATCH_SIZE = 1000;

    /* The time out of transaction */
    protected final static int TXN_TIME_OUT = 5000;

    private final static WriteOptions NO_SYNC_WRITE_OPTION =
            new WriteOptions(Durability.COMMIT_NO_SYNC, 0, null);

    /* Used to store wrapper table rows of statistics after a scan. */
    private final List<Row> statRows = new ArrayList<>();

    /* Flag to mark whether the scanning is stopped or not */
    protected volatile boolean stop;

    /* leaseManager is to determine whether extend a lease */
    private StatsLeaseManager<T> leaseManager;

    /* leaseInfo is to store the necessary info for the lease */
    private T leaseInfo;

    /**
     * latestVersion is to store the latest database version of the lease, it
     * is used to determine whether a lease is modified by another gathering
     * thread
     */
    private Version latestVersion;

    /* the interval between twice scanning */
    private final long scanInterval;

    /*
     * Build transaction configuration and cursor configuration, which are
     * used to scan the target database
     */
    protected static TransactionConfig txnConfig =
            new TransactionConfig().
            setDurability(com.sleepycat.je.Durability.READ_ONLY_TXN).
            setConsistencyPolicy(com.sleepycat.je.
                                 rep.NoConsistencyRequiredPolicy.
                                 NO_CONSISTENCY);

    /*
     * Set as READ_UNCOMMITTED, it is more efficient since the statistics
     * don't have to be 100% accurate.
     */
    protected static CursorConfig cursorConfig =
            new CursorConfig().setReadUncommitted(true);

    protected StatsScan(RepNode repNode,
                        TableAPI tableAPI,
                        StatsLeaseManager<T> leaseManager,
                        T leaseInfo,
                        long scanInterval,
                        Logger logger) {
        this.repNode = repNode;
        this.tableAPI = tableAPI;
        this.leaseManager = leaseManager;
        this.leaseInfo = leaseInfo;
        this.scanInterval = scanInterval;
        this.logger = logger;
    }

    protected void stop() {
        stop = true;
    }

    /**
     * Check whether the lease scan meets the condition of lease time and
     * interval time, and whether it can be started or not.
     * @return true if the scan can be started; or return false
     */
    private boolean checkLease() {
        /* Get a existing lease from database */
        final StatsLeaseManager<T>.Lease lease =
                leaseManager.getStoredLease(leaseInfo);

        if (lease == null) {
            /* No lease exists in database, try to create a new lease */
            Version version = leaseManager.createLease(leaseInfo);
            if (version == null) {
                logger.log(Level.INFO, "Get lease failed: another " +
                                       "ScanningThread already created " +
                                       "a lease for scanning the " +
                                       "selected partition");
                return false;
            }
            latestVersion = version;
            return true;
        }
        latestVersion = lease.getLatestVersion();

        /* When the lease exist, check whether the lease expires or not */
        final String expiryTimeStr = lease.getExpiryDate();
        long expiryTime;
        try {
            expiryTime = StatsLeaseManager.DATE_FORMAT.
                    parse(expiryTimeStr).getTime();
        } catch (ParseException e) {
            logger.log(Level.WARNING, "ScanningThread converts String into " +
                                      "Date failed: " + e);
            /*
             * Cannot convert String into Date, it is unexpected and cease
             * scanning
             */
            return false;
        }

        /* Lease is not expired and held by another StatsGather */
        if (System.currentTimeMillis() < expiryTime &&
            !lease.getLeaseRN().equals(leaseInfo.getLeaseRN())) {
            return false;
        }

        /* When the lease exists, check last updated date */
        final String lastUpdatedStr = lease.getLastUpdated();

        /*
         * Check lastUpdated when the lastUpdatedStr is not empty. If
         * lastUpdatedStr is empty, it means no previous scanning completes,
         * and the next scanning is the first scanning, so no need check the
         * lastUpdated
         */
        if (!lastUpdatedStr.isEmpty()) {
            long lastUpdated;
            try {
                lastUpdated = StatsLeaseManager.DATE_FORMAT.
                        parse(lastUpdatedStr).getTime();
            } catch (ParseException e) {
                logger.log(Level.WARNING, "ScanningThread converts String " +
                                          "into Date failed: " + e);

                /*
                 * Cannot convert String into Date, it is unexpected and cease
                 * scanning
                 */
                return false;
            }

            /* Current is in interval period, no need to start scanning */
            if (System.currentTimeMillis() < lastUpdated + scanInterval) {
                return false;
            }
        }

        /* If the lease expired, renew the lease. */
        Version version = leaseManager.renewLease(leaseInfo, latestVersion);
        if (version == null) {
            logger.log(Level.INFO, "Get lease failed: another " +
                                   "ScanningThread already renewed the lease");
            return false;
        }
        latestVersion = version;
        return true;
    }

    protected void runScan() throws Exception {
        try {

            /* Check whether we own the lease */
            if(!checkLease()) {
                return;
            }

            /* Start the scanning thread */
            logger.log(Level.FINE, "Lease acquired, scanning {0} starts",
                       leaseInfo);

            /*
             * When scan is stopped, the following statements are not
             * executed
             */
            if (stop) {
                return;
            }

            /* Do the task in the beginning of scan */
            if (!preScan()) {
                return;
            }

            /* Scan the database */
            boolean scanCompleted = scan();

            /* Do the task in the end of scan */
            postScan(scanCompleted);

            /*
             * When scan is stopped or scan is not completed, the following
             * statements are not executed
             */
            if (!scanCompleted || stop) {
                return;
            }

            Version version = leaseManager.extendLeaseIfNeeded(leaseInfo,
                    latestVersion);
            /*
             * Ensure that we have the lease before updating the stats. The
             * lease is owned by another thread, version is null and exit scan
             */
            if (version == null) {
                logger.log(Level.WARNING,
                           "Failed to extend statistics lease before stats " +
                           "table update");
                return;
            }

            /* Copy the latest version after extending lease */
            latestVersion = version;

            /* save the scanning result into statistics tables */
            saveResult();

            /*
             * Scan completed, modify the last updated time and terminate the
             * lease. Note that the lease must only be terminated after the
             * statistics have been updated by the runScan method.
             */
           version = leaseManager.terminateLease(leaseInfo, latestVersion);
           latestVersion = version;
           logger.log(Level.FINE, "Lease scanning {0} completed", leaseInfo);
        } catch (Exception e) {
            if (repNode.isStopped()) {
                logger.log(Level.INFO, "RepNode is stopped, " +
                            "statistics scanning exists: " + e);
            } else {
                throw e;
            }
        }
    }

    /**
     * Add a scan result into a list.
     *
     * @param statisticsInfo stored result, must not be null
     */
    protected void addRow(Row row) {
        statRows.add(row);
    }

    /**
     * Save all scan results into database.
     * @throws Exception
     */
    private void saveResult() throws Exception {
        logger.log(Level.FINE, "Store statistics information of into database");

        if (tableAPI == null) {
            logger.log(Level.WARNING, "Table API is invalid, store " +
                    "statistics information into database failed.");
            throw new IllegalStateException("Table API is invalid," +
                    "store statistics information into database failed");
        }

        /* When scan is stopped, the following statements are not executed */
        if (stop) {
            return;
        }

        for (final Row row : statRows) {
            try {
                tableAPI.put(row, null, NO_SYNC_WRITE_OPTION);
            } catch (DurabilityException | RequestTimeoutException e) {
                logger.log(Level.WARNING, "Exception found when put " + row, e);
            } catch (FaultException e) {
                logger.log(Level.WARNING, "FaultException found when put " +
                           row, e);
            } catch (Exception e) {
                throw e;
            }
        }
        statRows.clear();
    }

    /**
     * Check whether the statistics tables exist or not
     * @param metadata
     * @return true when all statistics tables exist; or return false.
     */
    protected abstract boolean checkStatsTable(TableMetadata metadata);

    /**
     * Accumulate result of every iteration scan.
     */
    protected abstract void accumulateResult(byte[] key);

    /**
     * Wrap the result of statistics as table rows and store the rows into
     * cache list
     */
    protected abstract void wrapResult();

    /**
     * Get the target database whose statistics information is scanned.
     * @return target database, the return result is always non-null
     */
    protected abstract Database getDatabase();

    /**
     * Allow to do some work before scan starts.
     */
    protected abstract boolean preScan();

    /**
     * Allow to do some work after scan completes.
     */
    protected abstract void postScan(boolean scanCompleted);

    /**
     * Scan the target database accumulating statistics
     * @return true if the scan completed, false, if it was incomplete.
     * If exceptions are thrown, the scan is interrupted.
     */
    private boolean scan() {
        final Database db = getDatabase();
        if (db == null) {
            throw new IllegalStateException("Database is null, scanning for " +
                                            leaseInfo + " exits");
        }

        TableMetadata metadata =
                (TableMetadata) repNode.getMetadata(MetadataType.TABLE);

        if (metadata == null) {
            throw new IllegalStateException("TableMetadata is null, " +
                                            "scanning for " + leaseInfo +
                                            " exits");
        }

        /* Stop scanning when statistics tables are missing */
        if (!checkStatsTable(metadata)) {
            throw new IllegalStateException("Statistics tables are missing, " +
                                            "scanning for " + leaseInfo +
                                            " exits");
        }

        if (!leaseManager.leaseTableExists()) {
            throw new IllegalStateException("Lease table not found");
        }

        final ReplicatedEnvironment repEnv =
                (ReplicatedEnvironment) db.getEnvironment();

        /* When scan is stopped, the following statements are not executed */
        if (stop) {
            return false;
        }

        logger.log(Level.FINE, "Start scanning database: " +
                db.getDatabaseName() + " to gather statistics");

        boolean hasMoreElements = true;

        /*  Scan the partition in several times */
        while(hasMoreElements) {
            /*
             * When scan is stopped, the following statements are not
             * executed
             */
            if (stop) {
                return false;
            }

            /* scan the target database an iteration */
            Version version =
                    leaseManager.extendLeaseIfNeeded(leaseInfo, latestVersion);
            /* Return false when the lease cannot be extended */
            if (version == null) {
                return false;
            }
            latestVersion = version;

            hasMoreElements = scanDatabase(repEnv, db);
        }

        /* When scan is stopped, the following statements are not executed */
        if (stop) {
            return false;
        }

        /* Wrap results as table rows */
        wrapResult();
        return true;
    }

    /**
     * Scan at most BATCH_SIZE kv pairs in the target database and put the
     * scanned data into the results IV.
     */
    protected abstract boolean scanDatabase(Environment env, Database db);
}
