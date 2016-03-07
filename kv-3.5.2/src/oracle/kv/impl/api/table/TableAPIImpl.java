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

package oracle.kv.impl.api.table;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.TimeUnit;

import oracle.kv.BulkWriteOptions;
import oracle.kv.Consistency;
import oracle.kv.Direction;
import oracle.kv.Durability;
import oracle.kv.DurabilityException;
import oracle.kv.EntryStream;
import oracle.kv.FaultException;
import oracle.kv.Key;
import oracle.kv.KeyRange;
import oracle.kv.Operation;
import oracle.kv.OperationExecutionException;
import oracle.kv.OperationFactory;
import oracle.kv.OperationResult;
import oracle.kv.ReturnValueVersion;
import oracle.kv.Value;
import oracle.kv.ValueVersion;
import oracle.kv.Version;
import oracle.kv.impl.api.KVStoreImpl;
import oracle.kv.impl.api.Request;
import oracle.kv.impl.api.bulk.BulkPut;
import oracle.kv.impl.api.ops.MultiDeleteTable;
import oracle.kv.impl.api.ops.MultiGetTable;
import oracle.kv.impl.api.ops.MultiGetTableKeys;
import oracle.kv.impl.api.ops.Result;
import oracle.kv.impl.api.ops.ResultKeyValueVersion;
import oracle.kv.impl.client.admin.DdlFuture;
import oracle.kv.impl.metadata.Metadata.MetadataType;
import oracle.kv.impl.rep.admin.RepNodeAdminAPI;
import oracle.kv.impl.topo.PartitionId;
import oracle.kv.impl.topo.RepNodeId;
import oracle.kv.impl.util.registry.RegistryUtils;
import oracle.kv.table.FieldRange;
import oracle.kv.table.IndexKey;
import oracle.kv.table.KeyPair;
import oracle.kv.table.MultiRowOptions;
import oracle.kv.table.PrimaryKey;
import oracle.kv.table.ReadOptions;
import oracle.kv.table.ReturnRow;
import oracle.kv.table.Row;
import oracle.kv.table.Table;
import oracle.kv.table.TableAPI;
import oracle.kv.table.TableIterator;
import oracle.kv.table.TableIteratorOptions;
import oracle.kv.table.TableOpExecutionException;
import oracle.kv.table.TableOperation;
import oracle.kv.table.TableOperationFactory;
import oracle.kv.table.TableOperationResult;
import oracle.kv.table.WriteOptions;

/**
 * Implementation of the TableAPI interface.  It also manages materialization
 * of tables from metadata and caches retrieved tables.
 *
 * TableAPIImpl maintains a cache of TableImpl tables that have been explicitly
 * fetched by TableImpl because of schema evolution.  If TableImpl encounters
 * a table version higher than its own then it will fetch that version so it
 * can deserialize records written from a later version.  It is assumed that
 * this cache will be small and is not used for user calls to getTable().
 */
public class TableAPIImpl implements TableAPI {
    final static String TABLE_PREFIX = "Tables";
    final static String SCHEMA_PREFIX = "Schemas";
    final static String SEPARATOR = ".";
    private final KVStoreImpl store;
    private final OpFactory opFactory;

    /*
     * Cache of TableImpl that have been fetched because the user's
     * table version is older than the latest table version.
     */
    final private ConcurrentHashMap<String, TableImpl> fetchedTables;

    /*
     * This must be public for KVStoreImpl to use it.
     */
    public TableAPIImpl(KVStoreImpl store) {
        this.store = store;
        opFactory = new OpFactory(store.getOperationFactory());
        fetchedTables = new ConcurrentHashMap<String, TableImpl>();
    }

    /*
     * Table metadata methods
     */
    @Override
    public Table getTable(String tableName)
    throws FaultException {

        /*
         * Only the last exception is kept.
         */
        Exception cause = null;
        TableImpl table = null;
        for (RepNodeId rnid : getRepNodeIds()) {
            try {
                cause = null;
                table = getTableFromRepNode(tableName, rnid);
                if (table != null) {
                    return table;
                }
            } catch (Exception e) {
                cause = e;
            }
        }
        if (cause != null) {
            String message = "Unable to find table " + tableName +
                ": " + cause.getMessage();
            throw new FaultException(message, cause, true);
        }
        return null;
    }

    @Override
    public Map<String, Table> getTables()
        throws FaultException {

        TableMetadata md = getTableMetadata();
        return md == null ? Collections.<String, Table>emptyMap() :
            md.getTables();
    }

    /**
     * Gets the TableMetadata object from a RepNode.
     * It is also used by the public getTables() interface.
     */
    public TableMetadata getTableMetadata()
        throws FaultException {

        /*
         * Only the last exception is kept.
         */
        Exception cause = null;
        TableMetadata md = null;
        for (RepNodeId rnid : getRepNodeIds()) {
            try {
                cause = null;
                md = getTableMetadataFromRepNode(rnid);
                if (md != null) {
                    return md;
                }
            } catch (Exception e) {
                cause = e;
            }
        }
        if (cause != null) {
            String message =
                "Unable to get table metadata:" + cause.getMessage();
            throw new FaultException(message, cause, true);
        }
        return null;
    }

    /*
     * Runtime interfaces
     */

    @Override
    public Row get(PrimaryKey rowKeyArg,
                   ReadOptions readOptions)
        throws FaultException {

        PrimaryKeyImpl rowKey = (PrimaryKeyImpl) rowKeyArg;
        Key key = rowKey.getPrimaryKey(false);
        ValueVersion vv = store.getInternal(key,
                                            rowKey.getTableImpl().getId(),
                                            getConsistency(readOptions),
                                            getTimeout(readOptions),
                                            getTimeoutUnit(readOptions));
        if (vv == null) {
            return null;
        }
        return getRowFromValueVersion(vv, rowKey, true);
    }

    @Override
    public Version put(Row rowArg,
                       ReturnRow prevRowArg,
                       WriteOptions writeOptions)
        throws FaultException {

        RowImpl row = (RowImpl) rowArg;
        ReturnValueVersion rvv = makeRVV(prevRowArg);
        Key key = row.getPrimaryKey(false);
        Value value = row.createValue();
        Version version = store.putInternal(key, value, rvv,
                                            row.getTableImpl().getId(),
                                            getDurability(writeOptions),
                                            getTimeout(writeOptions),
                                            getTimeoutUnit(writeOptions));
        initReturnRow(prevRowArg, row, rvv);
        return version;
    }

    @Override
    public Version putIfAbsent(Row rowArg,
                               ReturnRow prevRowArg,
                               WriteOptions writeOptions)
        throws FaultException {

        RowImpl row = (RowImpl) rowArg;
        ReturnValueVersion rvv = makeRVV(prevRowArg);
        Key key = row.getPrimaryKey(false);
        Value value = row.createValue();
        Version version =
            store.putIfAbsentInternal(key, value, rvv,
                                      row.getTableImpl().getId(),
                                      getDurability(writeOptions),
                                      getTimeout(writeOptions),
                                      getTimeoutUnit(writeOptions));
        initReturnRow(prevRowArg, row, rvv);
        return version;
    }

    @Override
    public Version putIfPresent(Row rowArg,
                                ReturnRow prevRowArg,
                                WriteOptions writeOptions)
        throws FaultException {

        RowImpl row = (RowImpl) rowArg;
        ReturnValueVersion rvv = makeRVV(prevRowArg);
        Key key = row.getPrimaryKey(false);
        Value value = row.createValue();
        Version version =
            store.putIfPresentInternal(key, value, rvv,
                                       row.getTableImpl().getId(),
                                       getDurability(writeOptions),
                                       getTimeout(writeOptions),
                                       getTimeoutUnit(writeOptions));
        initReturnRow(prevRowArg, row, rvv);
        return version;
    }

    @Override
    public Version putIfVersion(Row rowArg,
                                Version matchVersion,
                                ReturnRow prevRowArg,
                                WriteOptions writeOptions)
        throws FaultException {

        RowImpl row = (RowImpl) rowArg;
        ReturnValueVersion rvv = makeRVV(prevRowArg);
        Key key = row.getPrimaryKey(false);
        Value value = row.createValue();
        Version version =
            store.putIfVersionInternal(key, value,
                                       matchVersion, rvv,
                                       row.getTableImpl().getId(),
                                       getDurability(writeOptions),
                                       getTimeout(writeOptions),
                                       getTimeoutUnit(writeOptions));
        initReturnRow(prevRowArg, row, rvv);
        return version;
    }


    @Override
    public void put(List<EntryStream<Row>> rowStreams,
                    BulkWriteOptions writeOptions) {

        if (rowStreams == null || rowStreams.isEmpty()) {
            throw new IllegalArgumentException("The input streams argument " +
                    "should not be null or empty.");
        }

        final BulkWriteOptions options;
        if (writeOptions != null) {
            options = writeOptions;
        } else {
            options =
                new BulkWriteOptions(TableAPIImpl.getDurability(writeOptions),
                                     TableAPIImpl.getTimeout(writeOptions),
                                     TableAPIImpl.getTimeoutUnit(writeOptions));
        }
        final BulkPut<Row> bulkPut =
            new BulkPut<Row>(store, options, rowStreams, store.getLogger()) {

                @Override
                public BulkPut<Row>.StreamReader<Row>
                    createReader(int streamId, EntryStream<Row> stream) {
                    return new StreamReader<Row>(streamId, stream) {

                        @Override
                        protected Key getKey(Row row) {
                            return ((RowImpl)row).getPrimaryKey(false);
                        }

                        @Override
                        protected Value getValue(Row row) {
                            return ((RowImpl)row).createValue();
                        }

                        @Override
                        protected long getTableId(Row row) {
                            return ((TableImpl)row.getTable()).getId();
                        }
                    };
                }

                @Override
                protected Row convertToEntry(Key key, Value value) {
                    final byte[] keyBytes =
                        store.getKeySerializer().toByteArray(key);
                    final TableImpl table = (TableImpl)findTableByKey(keyBytes);
                    if (table == null) {
                        return null;
                    }
                    final RowImpl row =
                        table.createPrimaryKeyFromKeyBytes(keyBytes);
                    assert(row != null);
                    final ValueVersion vv = new ValueVersion(value, null);
                    return row.rowFromValueVersion(vv, false);
                }

                private Table findTableByKey(final byte[] keyBytes) {
                    final Map<String, Table> tables = getTables();
                    for (Entry<String, Table> entry : tables.entrySet()) {
                        final TableImpl table = (TableImpl)entry.getValue();
                        final Table target = table.findTargetTable(keyBytes);
                        if (target != null) {
                            return target;
                        }
                    }
                    return null;
                }
        };

        try {
            bulkPut.execute();
        } catch (InterruptedException e) {
            throw new IllegalStateException("Unexpected interrupt during " +
            		"putBulk()", e);
        }
    }

    /**
     * Deprecated in favor of KVStore.execute. Delegate over to that newer
     * method.
     */
    @Deprecated
    @Override
    public oracle.kv.table.ExecutionFuture execute(String statement)
            throws IllegalArgumentException, FaultException {
        return new DeprecatedResults.ExecutionFutureWrapper(store.execute(statement));
    }

    @Deprecated
    @Override
    public oracle.kv.table.StatementResult executeSync(String statement)
        throws FaultException {
        return new DeprecatedResults.StatementResultWrapper
                (store.executeSync(statement));
    }

    @Deprecated
    @Override
    public oracle.kv.table.ExecutionFuture getFuture(int planId) {
        if (planId < 1) {
            throw new IllegalArgumentException("PlanId " + planId +
                                               " isn't valid, must be > 1");
        }
        byte[] futureBytes = DdlFuture.toByteArray(planId);
        return new DeprecatedResults.ExecutionFutureWrapper
                (store.getFuture(futureBytes));
    }

    /*
     * Multi/iterator ops
     */
    @Override
    public List<Row> multiGet(PrimaryKey rowKeyArg,
                              MultiRowOptions getOptions,
                              ReadOptions readOptions)
        throws FaultException {

        boolean hasAncestorTables = false;
        PrimaryKeyImpl rowKey = (PrimaryKeyImpl) rowKeyArg;
        Table table = rowKey.getTable();
        TableKey key = TableKey.createKey(table, rowKey, true);
        if (!key.getMajorKeyComplete()) {
            throw new IllegalArgumentException
                ("Cannot perform multiGet on a primary key without a " +
                 "complete major path");
        }

        if (getOptions != null) {
            validateMultiRowOptions(getOptions, table, false);
            if (getOptions.getIncludedParentTables() != null) {
                hasAncestorTables = true;
            }
        }
        /* Execute request. */
        final byte[] parentKeyBytes =
            store.getKeySerializer().toByteArray(key.getKey());
        final PartitionId partitionId =
            store.getDispatcher().getPartitionId(parentKeyBytes);
        final MultiGetTable get =
            new MultiGetTable(parentKeyBytes,
                              makeTargetTables(table, getOptions),
                              makeKeyRange(key, getOptions));
        final Request req =
            store.makeReadRequest(get, partitionId,
                                  getConsistency(readOptions),
                                  getTimeout(readOptions),
                                  getTimeoutUnit(readOptions));
        final Result result = store.executeRequest(req);

        /*
         * Convert results to List<Row>
         */
        return processMultiResults(table, result, hasAncestorTables);
    }

    @Override
    public List<PrimaryKey> multiGetKeys(PrimaryKey rowKeyArg,
                                         MultiRowOptions getOptions,
                                         ReadOptions readOptions)
        throws FaultException {

        boolean hasAncestorTables = false;
        PrimaryKeyImpl rowKey = (PrimaryKeyImpl) rowKeyArg;
        Table table = rowKey.getTable();
        TableKey key = TableKey.createKey(table, rowKey, true);
        if (!key.getMajorKeyComplete()) {
            throw new IllegalArgumentException
                ("Cannot perform multiGet on a primary key without a " +
                 "complete major path");
        }

        if (getOptions != null) {
            validateMultiRowOptions(getOptions, table, false);
            if (getOptions.getIncludedParentTables() != null) {
                hasAncestorTables = true;
            }
        }
        /* Execute request. */
        final byte[] parentKeyBytes =
            store.getKeySerializer().toByteArray(key.getKey());
        final PartitionId partitionId =
            store.getDispatcher().getPartitionId(parentKeyBytes);
        final MultiGetTableKeys get =
            new MultiGetTableKeys(parentKeyBytes,
                                  makeTargetTables(table, getOptions),
                                  makeKeyRange(key, getOptions));
        final Request req =
            store.makeReadRequest(get, partitionId,
                                  getConsistency(readOptions),
                                  getTimeout(readOptions),
                                  getTimeoutUnit(readOptions));
        final Result result = store.executeRequest(req);

        /*
         * Convert byte[] keys to Key objects.
         */
        final List<byte[]> byteKeyResults = result.getKeyList();
        assert result.getSuccess() == (!byteKeyResults.isEmpty());
        return processMultiResults(table, byteKeyResults, hasAncestorTables);
    }

    @Override
    public TableIterator<Row> tableIterator(PrimaryKey rowKeyArg,
                                            MultiRowOptions getOptions,
                                            TableIteratorOptions iterateOptions)
        throws FaultException {
        return tableIterator(rowKeyArg, getOptions, iterateOptions, null);
    }

    /**
     * @hidden
     */
    public TableIterator<Row> tableIterator(PrimaryKey rowKeyArg,
                                            MultiRowOptions getOptions,
                                            TableIteratorOptions iterateOptions,
                                            Set<Integer> partitions)
        throws FaultException {

        final PrimaryKeyImpl rowKey = (PrimaryKeyImpl) rowKeyArg;
        final Table table = rowKey.getTable();
        final TableKey key = TableKey.createKey(table, rowKey, true);


        if (getOptions != null) {
            validateMultiRowOptions(getOptions, table, false);
        }
        return TableScan.createTableIterator(this, key, getOptions,
                                             iterateOptions, partitions);
    }

    @Override
    public TableIterator<PrimaryKey> tableKeysIterator
        (PrimaryKey rowKeyArg,
         MultiRowOptions getOptions,
         TableIteratorOptions iterateOptions)
        throws FaultException {

        final PrimaryKeyImpl rowKey = (PrimaryKeyImpl) rowKeyArg;
        final Table table = rowKey.getTable();
        final TableKey key = TableKey.createKey(table, rowKey, true);


        if (getOptions != null) {
            validateMultiRowOptions(getOptions, table, false);
        }
        return TableScan.
            createTableKeysIterator(this, key, getOptions, iterateOptions);
    }

    @Override
    public boolean delete(PrimaryKey rowKeyArg,
                          ReturnRow prevRowArg,
                          WriteOptions writeOptions)
        throws FaultException {

        PrimaryKeyImpl rowKey = (PrimaryKeyImpl) rowKeyArg;
        ReturnValueVersion rvv = makeRVV(prevRowArg);
        Key key = rowKey.getPrimaryKey(false);
        boolean retval = store.deleteInternal(key, rvv,
                                              getDurability(writeOptions),
                                              getTimeout(writeOptions),
                                              getTimeoutUnit(writeOptions),
                                              rowKey.getTableImpl().getId());
        initReturnRow(prevRowArg, rowKey, rvv);
        return retval;
    }

    @Override
    public boolean deleteIfVersion(PrimaryKey rowKeyArg,
                                   Version matchVersion,
                                   ReturnRow prevRowArg,
                                   WriteOptions writeOptions)
        throws FaultException {

        PrimaryKeyImpl rowKey = (PrimaryKeyImpl) rowKeyArg;
        ReturnValueVersion rvv = makeRVV(prevRowArg);
        Key key = rowKey.getPrimaryKey(false);
        boolean retval = store.deleteIfVersionInternal
            (key, matchVersion, rvv,
             getDurability(writeOptions),
             getTimeout(writeOptions),
             getTimeoutUnit(writeOptions),
             rowKey.getTableImpl().getId());

        initReturnRow(prevRowArg, rowKey, rvv);
        return retval;
    }

    @Override
    public int multiDelete(PrimaryKey rowKeyArg,
                           MultiRowOptions getOptions,
                           WriteOptions writeOptions)
        throws FaultException {

        PrimaryKeyImpl rowKey = (PrimaryKeyImpl) rowKeyArg;
        Table table = rowKey.getTable();
        TableKey key = TableKey.createKey(table, rowKey, true);
        if (!key.getMajorKeyComplete()) {
            throw new IllegalArgumentException
                ("Cannot perform multiDelete on a primary key without a " +
                 "complete major path.  Key: " + rowKey.toJsonString(false));
        }

        if (getOptions != null) {
            validateMultiRowOptions(getOptions, table, false);
        }
        final KeyRange keyRange = makeKeyRange(key, getOptions);

        /* Execute request. */
        final byte[] parentKeyBytes =
            store.getKeySerializer().toByteArray(key.getKey());
        final PartitionId partitionId =
            store.getDispatcher().getPartitionId(parentKeyBytes);
        final MultiDeleteTable del =
            new MultiDeleteTable(parentKeyBytes,
                                 makeTargetTables(table, getOptions),
                                 keyRange);
        final Request req =
            store.makeWriteRequest(del, partitionId,
                                   getDurability(writeOptions),
                                   getTimeout(writeOptions),
                                   getTimeoutUnit(writeOptions));
        final Result result = store.executeRequest(req);
        return result.getNDeletions();
    }

    /*
     * Index iterator operations
     */
    @Override
    public TableIterator<Row> tableIterator(IndexKey indexKeyArg,
                                            MultiRowOptions getOptions,
                                            TableIteratorOptions iterateOptions)
        throws FaultException {
        final  IndexKeyImpl indexKey = (IndexKeyImpl)indexKeyArg;
        if (getOptions != null) {
            validateMultiRowOptions(getOptions, indexKey.getTable(), true);
        }
        return IndexScan.createTableIterator(this,
                                             indexKey,
                                             getOptions,
                                             iterateOptions);
    }

    @Override
    public TableIterator<KeyPair>
            tableKeysIterator(IndexKey indexKeyArg,
                              MultiRowOptions getOptions,
                              TableIteratorOptions iterateOptions)
        throws FaultException {
        final  IndexKeyImpl indexKey = (IndexKeyImpl)indexKeyArg;
        if (getOptions != null) {
            validateMultiRowOptions(getOptions, indexKey.getTable(), true);
        }
        return IndexScan.createTableKeysIterator(this,
                                                 indexKey,
                                                 getOptions,
                                                 iterateOptions);
    }

    @Override
    public TableOperationFactory getTableOperationFactory() {
        return opFactory;
    }

    @Override
    public TableIterator<Row>
        tableIterator(Iterator<PrimaryKey> primaryKeyIterator,
                      MultiRowOptions getOptions,
                      TableIteratorOptions iterateOptions) {

        if (primaryKeyIterator == null) {
            throw new IllegalArgumentException("Parent key iterator should " +
                "not be null");
        }

        final List<Iterator<PrimaryKey>> keyIterators =
            Arrays.asList(primaryKeyIterator);
        return tableIterator(keyIterators, getOptions, iterateOptions);
    }

    @Override
    public TableIterator<PrimaryKey>
        tableKeysIterator(Iterator<PrimaryKey> primaryKeyIterator,
                          MultiRowOptions getOptions,
                          TableIteratorOptions iterateOptions) {

        if (primaryKeyIterator == null) {
            throw new IllegalArgumentException("Parent key iterator should " +
                "not be null");
        }

        final List<Iterator<PrimaryKey>> keyIterators =
            Arrays.asList(primaryKeyIterator);
        return tableKeysIterator(keyIterators, getOptions, iterateOptions);
    }

    @Override
    public TableIterator<Row>
        tableIterator(List<Iterator<PrimaryKey>> primaryKeyIterators,
                      MultiRowOptions getOptions,
                      TableIteratorOptions iterateOptions)

        throws FaultException {

        if (primaryKeyIterators == null || primaryKeyIterators.isEmpty()) {
            throw new IllegalArgumentException("Parent key iterators should " +
                "not be null or empty");
        }

        if (iterateOptions != null &&
            iterateOptions.getDirection() != Direction.UNORDERED) {
            throw new IllegalArgumentException("Direction must be " +
                "Direction.UNORDERED for this operation");
        }

        return new TableMultiGetBatch(this, primaryKeyIterators,
                                      getOptions, iterateOptions)
                .createIterator();
    }

    @Override
    public TableIterator<PrimaryKey>
        tableKeysIterator(List<Iterator<PrimaryKey>> parentKeyiterators,
                          MultiRowOptions getOptions,
                          TableIteratorOptions iterateOptions)
        throws FaultException {

        if (parentKeyiterators == null || parentKeyiterators.isEmpty()) {
            throw new IllegalArgumentException("Parent key iterators should " +
                "not be null or empty");
        }

        if (iterateOptions != null &&
            iterateOptions.getDirection() != Direction.UNORDERED) {
            throw new IllegalArgumentException("Direction must be " +
                "Direction.UNORDERED for this operation");
        }

        return new TableMultiGetBatch(this, parentKeyiterators,
                                      getOptions, iterateOptions)
                .createKeysIterator();
    }

    /**
     * All of the TableOperations can be directly mapped to simple KV operations
     * so do that.
     */
    @Override
    public List<TableOperationResult> execute(List<TableOperation> operations,
                                              WriteOptions writeOptions)
        throws TableOpExecutionException,
               DurabilityException,
               FaultException {

        ArrayList<Operation> opList =
            new ArrayList<Operation>(operations.size());
        for (TableOperation op : operations) {
            opList.add(((OpWrapper)op).getOperation());
        }
        List<OperationResult> results;

        try {
            results = store.execute(opList,
                          getDurability(writeOptions),
                          getTimeout(writeOptions),
                          getTimeoutUnit(writeOptions));
            List<TableOperationResult> tableResults =
                    new ArrayList<TableOperationResult>(results.size());
            int index = 0;
            for (OperationResult opRes : results) {
                PrimaryKey pkey = operations.get(index).getPrimaryKey();
                tableResults.add(new OpResultWrapper(this, opRes, pkey));
                ++index;
            }
            return tableResults;
        } catch (OperationExecutionException e) {
            /* Convert this to a TableOpExecutionException */
            int failedOpIndex = e.getFailedOperationIndex();
            PrimaryKey pkey = operations.get(failedOpIndex).getPrimaryKey();
            OperationResult result = e.getFailedOperationResult();
            TableOperationResult failedResult =
                    new OpResultWrapper(this, result, pkey);

            throw new TableOpExecutionException(operations.get(failedOpIndex),
                                                failedOpIndex,
                                                failedResult);
        }
    }

    /**
     * Creates a Row from the Value with a retry in the case of a
     * TableVersionException.
     */
    RowImpl getRowFromValueVersion(ValueVersion vv, RowImpl row,
                                   boolean keyOnly) {
        int requiredVersion = 0;
        try {
            return row.rowFromValueVersion(vv, keyOnly);
        } catch (TableVersionException tve) {
            requiredVersion = tve.getRequiredVersion();
            assert requiredVersion > row.getTable().getTableVersion();
        }

        /*
         * Fetch the required table, create a new row from the existing
         * row and try again.  The fetch will throw if the table and version
         * can't be found.
         */
        TableImpl newTable = fetchTable(row.getTable().getFullName(),
                                        requiredVersion);
        assert requiredVersion == newTable.getTableVersion();

        /*
         * Set the version of the table to the original version to ensure that
         * deserialization does the right thing with added and removed fields.
         */
        newTable =
            (TableImpl) newTable.getVersion(row.getTable().getTableVersion());
        RowImpl newRow = newTable.createRow(row);
        return newRow.rowFromValueVersion(vv, keyOnly);
    }

    TableImpl fetchTable(String tableName, int tableVersion) {
        TableImpl table = fetchedTables.get(tableName);
        if (table != null && table.numTableVersions() >= tableVersion) {
            return (TableImpl) table.getVersion(tableVersion);
        }

        /*
         * Either the table is not in the cache or it is not sufficiently
         * recent.  Go to the server.
         */
        table = (TableImpl) getTable(tableName);
        if (table != null && table.numTableVersions() >= tableVersion) {

            /*
             * Cache the table.  If an intervening operation cached the
             * table, make sure that the cache has the lastest version.
             */
            TableImpl t = fetchedTables.putIfAbsent(tableName, table);
            if (t != null && table.numTableVersions() > t.numTableVersions()) {
                fetchedTables.put(tableName, table);
            }
            return (TableImpl) table.getVersion(tableVersion);
        }
        throw new IllegalArgumentException
            ("Table or version does not exist.  It may have been removed: " +
             tableName + ", version " + tableVersion);
    }

    public KVStoreImpl getStore() {
        return store;
    }

    /**
     * The next classes implement mapping of TableOperation and
     * TableOperationFactory to the KVStore Operation and OperationFactory.
     */
    private static class OpWrapper implements TableOperation {
        private final Operation op;
        private final TableOperation.Type type;
        private final RowImpl record;

        private OpWrapper(Operation op, TableOperation.Type type,
                          final RowImpl record) {
            this.op = op;
            this.type = type;
            this.record = record;
        }

        @Override
        public Row getRow() {
            return record;
        }

        @Override
        public PrimaryKey getPrimaryKey() {
            if (record instanceof PrimaryKey) {
                return (PrimaryKey) record;
            }
            return record.createPrimaryKey();
        }

        @Override
        public TableOperation.Type getType() {
            return type;
        }

        @Override
        public boolean getAbortIfUnsuccessful() {
            return op.getAbortIfUnsuccessful();
        }

        private Operation getOperation() {
            return op;
        }
    }

    private static class OpResultWrapper implements TableOperationResult {
        private final TableAPIImpl impl;
        private final OperationResult opRes;
        private final PrimaryKey key;

        private OpResultWrapper(TableAPIImpl impl,
                                OperationResult opRes, PrimaryKey key) {
            this.impl = impl;
            this.opRes = opRes;
            this.key = key;
        }

        @Override
        public Version getNewVersion() {
            return opRes.getNewVersion();
        }

        @Override
        public Row getPreviousRow() {
            Value value = opRes.getPreviousValue();
            if (value != null && key != null) {
                return impl.getRowFromValueVersion
                    (new ValueVersion(value, null), (RowImpl) key, true);
            }
            return null;
        }

        @Override
        public Version getPreviousVersion() {
            return opRes.getPreviousVersion();
        }

        @Override
        public boolean getSuccess() {
            return opRes.getSuccess();
        }
    }

    private static class OpFactory implements TableOperationFactory {
        private final OperationFactory factory;

        private OpFactory(final OperationFactory factory) {
            this.factory = factory;
        }

        @Override
        public TableOperation createPut(Row rowArg,
                                        ReturnRow.Choice prevReturn,
                                        boolean abortIfUnsuccessful) {

            RowImpl row = (RowImpl) rowArg;
            ReturnValueVersion.Choice choice =
                ReturnRowImpl.mapChoice(prevReturn);
            Key key = row.getPrimaryKey(false);
            Value value = row.createValue();
            Operation op = factory.createPut(key, value, choice,
                                             abortIfUnsuccessful);
            return new OpWrapper(op, TableOperation.Type.PUT, row);
        }

        @Override
        public TableOperation createPutIfAbsent(Row rowArg,
                                                ReturnRow.Choice prevReturn,
                                                boolean abortIfUnsuccessful) {

            RowImpl row = (RowImpl) rowArg;
            ReturnValueVersion.Choice choice =
                ReturnRowImpl.mapChoice(prevReturn);
            Key key = row.getPrimaryKey(false);
            Value value = row.createValue();
            Operation op = factory.createPutIfAbsent(key, value, choice,
                                                     abortIfUnsuccessful);
            return new OpWrapper(op, TableOperation.Type.PUT_IF_ABSENT, row);
        }

        @Override
        public TableOperation createPutIfPresent(Row rowArg,
                                                 ReturnRow.Choice prevReturn,
                                                 boolean abortIfUnsuccessful) {

            RowImpl row = (RowImpl) rowArg;
            ReturnValueVersion.Choice choice =
                ReturnRowImpl.mapChoice(prevReturn);
            Key key = row.getPrimaryKey(false);
            Value value = row.createValue();
            Operation op = factory.createPutIfPresent(key, value, choice,
                                                     abortIfUnsuccessful);
            return new OpWrapper(op, TableOperation.Type.PUT_IF_PRESENT, row);
        }

        @Override
        public TableOperation createPutIfVersion(Row rowArg,
                                                 Version versionMatch,
                                                 ReturnRow.Choice prevReturn,
                                                 boolean abortIfUnsuccessful) {

            RowImpl row = (RowImpl) rowArg;
            ReturnValueVersion.Choice choice =
                ReturnRowImpl.mapChoice(prevReturn);
            Key key = row.getPrimaryKey(false);
            Value value = row.createValue();
            Operation op = factory.createPutIfVersion(key, value,
                                                      versionMatch, choice,
                                                      abortIfUnsuccessful);
            return new OpWrapper(op, TableOperation.Type.PUT_IF_VERSION, row);
        }

        @Override
        public TableOperation createDelete
            (PrimaryKey keyArg,
             ReturnRow.Choice prevReturn,
             boolean abortIfUnsuccessful) {

            PrimaryKeyImpl rowKey = (PrimaryKeyImpl) keyArg;
            ReturnValueVersion.Choice choice =
                ReturnRowImpl.mapChoice(prevReturn);
            Key key = rowKey.getPrimaryKey(false);
            Operation op = factory.createDelete(key, choice,
                                                abortIfUnsuccessful);
            return new OpWrapper(op, TableOperation.Type.DELETE, rowKey);
        }

        @Override
        public TableOperation createDeleteIfVersion
            (PrimaryKey keyArg,
             Version versionMatch,
             ReturnRow.Choice prevReturn,
             boolean abortIfUnsuccessful) {

            PrimaryKeyImpl rowKey = (PrimaryKeyImpl) keyArg;
            ReturnValueVersion.Choice choice =
                ReturnRowImpl.mapChoice(prevReturn);
            Key key = rowKey.getPrimaryKey(false);
            Operation op = factory.createDeleteIfVersion(key, versionMatch,
                                                         choice,
                                                         abortIfUnsuccessful);
            return new OpWrapper
                (op, TableOperation.Type.DELETE_IF_VERSION, rowKey);
        }
    }

    /************* end runtime methods **************/

    /*
     * Internal utilities
     */

    /**
     * Returns list of RepNodeIds to use for methods that need to iterate
     * them to get metadata.
     */
    private Set<RepNodeId> getRepNodeIds() {
        return store.getDispatcher().getTopologyManager().
            getTopology().getRepNodeIds();
    }

    /**
     * Goes to the specified RN and returns the table associated with the table
     * name.   A failure may be either a null return value or an exception.  The
     * caller needs to handle exceptions.
     */
    private TableImpl getTableFromRepNode(String tableName,
                                          RepNodeId rnid)
        throws Exception {

        final RegistryUtils regUtils = store.getDispatcher().getRegUtils();
        if (regUtils == null) {
            return null;
        }
        final RepNodeAdminAPI repNodeAdmin = regUtils.getRepNodeAdmin(rnid);
        if (repNodeAdmin != null) {
            return (TableImpl)repNodeAdmin.getMetadata
                (MetadataType.TABLE,
                 new TableMetadata.TableMetadataKey
                 (tableName).
                 getMetadataKey(), 0);
        }
        return null;
    }

    /**
     * Returns the TableMetadata object from the specified RepNode.  A failure
     * may be either a null return value or an exception.  The caller needs to
     * handle exceptions.
     */
    private TableMetadata getTableMetadataFromRepNode
        (RepNodeId rnid) throws Exception {

        final RegistryUtils regUtils = store.getDispatcher().getRegUtils();
        if (regUtils == null) {
            return null;
        }
        final RepNodeAdminAPI repNodeAdmin = regUtils.getRepNodeAdmin(rnid);
        if (repNodeAdmin != null) {
            return (TableMetadata) repNodeAdmin.
                getMetadata(MetadataType.TABLE);
        }
        return null;
    }

    private ReturnValueVersion makeRVV(ReturnRow rr) {
        if (rr != null) {
            return ((ReturnRowImpl)rr).makeReturnValueVersion();
        }
        return null;
    }

    private void initReturnRow(ReturnRow rr, RowImpl key,
                               ReturnValueVersion rvv) {
        if (rr != null) {
            ((ReturnRowImpl)rr).init(this, rvv, key);
        }
    }

    static KeyRange makeKeyRange(TableKey key, MultiRowOptions getOptions) {
        if (getOptions != null) {
            FieldRange range = getOptions.getFieldRange();
            if (range != null) {
                if (key.getKeyComplete()) {
                    throw new IllegalArgumentException
                        ("Cannot specify a FieldRange with a complete " +
                         "primary key");
                }
                key.validateFieldOrder(range);
                return createKeyRange(range);
            }
        } else {
            key.getRow().validate();
        }
        return null;
    }

    static KeyRange createKeyRange(FieldRange range) {
        if (range == null) {
            return null;
        }

        String start = null;
        String end = null;
        boolean startInclusive = true;
        boolean endInclusive = true;
        if (range.getStart() != null) {
            start = ((FieldValueImpl)range.getStart()).
                formatForKey(range.getDefinition());
            startInclusive = range.getStartInclusive();
        }
        if (range.getEnd() != null) {
            end = ((FieldValueImpl)range.getEnd()).
                formatForKey(range.getDefinition());
            endInclusive = range.getEndInclusive();
        }
        return new KeyRange(start, startInclusive, end, endInclusive);
    }

    /**
     * Turn a List<byte[]> of keys into List<PrimaryKey>
     */
    private List<PrimaryKey>
        processMultiResults(Table table,
                            List<byte[]> keys,
                            boolean hasAncestorTables) {
        List<PrimaryKey> list = new ArrayList<PrimaryKey>(keys.size());
        TableImpl t = (TableImpl) table;
        if (hasAncestorTables) {
            t = t.getTopLevelTable();
        }
        for (byte[] key : keys) {
            PrimaryKeyImpl pk =
                t.createPrimaryKeyFromKeyBytes(key);
            if (pk != null) {
                list.add(pk);
            }
        }
        return list;
    }

    /**
     * Turn a List<ResultKeyValueVersion> of results into List<Row>
     */
    private List<Row>
        processMultiResults(Table table,
                            Result result,
                            boolean hasAncestorTables) {
        final List<ResultKeyValueVersion> resultList =
            result.getKeyValueVersionList();
        List<Row> list = new ArrayList<Row>(resultList.size());
        TableImpl t = (TableImpl) table;
        if (hasAncestorTables) {
            t = t.getTopLevelTable();
        }

        for (ResultKeyValueVersion rkvv : result.getKeyValueVersionList()) {
            RowImpl row = t.createRowFromKeyBytes(rkvv.getKeyBytes());
            if (row != null) {
                ValueVersion vv = new ValueVersion(rkvv.getValue(),
                                                   rkvv.getVersion());
                list.add(getRowFromValueVersion(vv, row, false));
            }
        }
        return list;
    }

    /**
     * Validate the ancestor and child tables, if set against the target table.
     */
    static void validateMultiRowOptions(MultiRowOptions mro,
                                        Table targetTable,
                                        boolean isIndex) {
        if (mro.getIncludedParentTables() != null) {
            for (Table t : mro.getIncludedParentTables()) {
                if (!((TableImpl)targetTable).isAncestor(t)) {
                    throw new IllegalArgumentException
                        ("Ancestor table \"" + t.getFullName() + "\" is not " +
                         "an ancestor of target table \"" +
                         targetTable.getFullName() + "\"");
                }
            }
        }
        if (mro.getIncludedChildTables() != null) {
            if (isIndex) {
                throw new UnsupportedOperationException
                    ("Child table returns are not supported for index " +
                     "scan operations");
            }
            for (Table t : mro.getIncludedChildTables()) {
                if (!((TableImpl)t).isAncestor(targetTable)) {
                    throw new IllegalArgumentException
                        ("Child table \"" + t.getFullName() + "\" is not a " +
                         "descendant of target table \"" +
                         targetTable.getFullName() + "\"");
                }

            }
        }
    }

    static Consistency getConsistency(ReadOptions opts) {
        return (opts != null ? opts.getConsistency() : null);
    }

    static long getTimeout(ReadOptions opts) {
        return (opts != null ? opts.getTimeout() : 0);
    }

    static TimeUnit getTimeoutUnit(ReadOptions opts) {
        return (opts != null ? opts.getTimeoutUnit() : null);
    }

    static Direction getDirection(TableIteratorOptions opts,
                                  TableKey key) {
        if (opts == null) {
           return key.getMajorKeyComplete() ? Direction.FORWARD :
                                              Direction.UNORDERED;
        }
        return opts.getDirection();
    }

    static int getBatchSize(TableIteratorOptions opts) {
        return ((opts != null && opts.getResultsBatchSize() != 0) ?
                opts.getResultsBatchSize():
                KVStoreImpl.DEFAULT_ITERATOR_BATCH_SIZE);
    }

    static Durability getDurability(WriteOptions opts) {
        return (opts != null ? opts.getDurability() : null);
    }

    static long getTimeout(WriteOptions opts) {
        return (opts != null ? opts.getTimeout() : 0);
    }

    static TimeUnit getTimeoutUnit(WriteOptions opts) {
        return (opts != null ? opts.getTimeoutUnit() : null);
    }

    static TargetTables makeTargetTables(Table target,
                                         MultiRowOptions getOptions) {
        List<Table> childTables =
            getOptions != null ? getOptions.getIncludedChildTables() : null;
        List<Table> ancestorTables =
            getOptions != null ? getOptions.getIncludedParentTables() : null;

        return new TargetTables(target, childTables, ancestorTables);
    }
}
