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

package oracle.kv.impl.api.bulk;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;
import java.util.logging.Level;
import java.util.logging.Logger;

import oracle.kv.BulkWriteOptions;
import oracle.kv.EntryStream;
import oracle.kv.FaultException;
import oracle.kv.Key;
import oracle.kv.Value;
import oracle.kv.impl.api.KVStoreImpl;
import oracle.kv.impl.api.KeySerializer;
import oracle.kv.impl.topo.PartitionId;
import oracle.kv.impl.topo.RepGroupId;
import oracle.kv.impl.topo.Topology;
import oracle.kv.impl.topo.TopologyUtil;
import oracle.kv.impl.util.KVThreadFactory;

/**
 * This class represents a single bulk put operation. It provides the common
 * underpinnings for batching both rows and KV pairs.
 *
 * The overall flow of entries is designed to:
 *
 * 1) Utilize the store as completely as possible by ensuring that each shard
 * is kept uniformly busy at all times.
 *
 * 2) Make best use of thread level parallelism.
 *
 * 3) Assemble batches so that key value pairs in a batch are clustered by
 * partition and are sorted within each partition to optimize JE insert
 * performance.
 *
 * The general flow of an entry is as follows:
 *
 * 1) The entry is supplied by the user supplied stream. Multiple streams may
 * be read in parallel by the BuldStreamReader task associated with the stream,
 * depending on the configurable level of stream parallelization.
 *
 * 2) Each StreamReader accumulates the new entry, along with earlier
 * entries, in a sorted tree associated with each partition.
 *
 * 3) When the storage threshold associated with the partition is exceeded, the
 * the leading elements in the sorted tree are assembled into a batch and
 * placed into a queue associated with the partition.
 *
 * 4) The ShardPutTask associated with the queue the takes the batch and writes
 * it to the shard.
 *
 * @param <T> must be a Row or a KV pair
 */
public abstract class BulkPut<T> {

    /**
     * Handle to the store
     */
    private final KVStoreImpl store;

    /**
     * The topology associated with the store.
     */
    private final Topology topology;

    /**
     * The key serializer associated with the store.
     */
    private final KeySerializer serializer;

    /**
     * The streams supplying the entries to be loaded.
     */
    private final EntryStream<T> streams[];

    /**
     * The list of stream reader that read the entries supplied by stream.
     */
    private final List<StreamReader<T>> readers;

    /**
     * A map indexed by partition id which yields the values that are being
     * aggregated for that partition.
     */
    private final PartitionValues pMap[];

    /**
     * The Key comparator used to group KV pairs associated with a
     * partition, so that they can be sent as a contiguous batch. This is the
     * same comparator used to insert KV pairs into the JE btree.
     */
    public final static Comparator<byte[]> KEY_BYTES_COMPARATOR =
        new Key.BytesComparator();

    /**
     * The options in effect for this operation.
     */
    private final BulkWriteOptions options;

    /**
     * This represents the threshold bytes that will be aggregated for each
     * partition, before the bytes are flushed out to the shard holding the
     * partition.
     */
    private final long partitionThresholdBytes;

    /**
     * The min number of bytes required per partition for batching to be
     * effective.
     */
    private static final int partitionHeapMinBytes = 100 * 1024;

    /**
     * Used to hold the aggregate statistics associated with this operation
     */
    final AggregateStatistics statistics = new AggregateStatistics();

    /**
     * The logger to be used on the client.
     */
    private final Logger logger;

    /**
     * The exception to terminate the bulk put operation.
     */
    private final AtomicReference<Exception> terminateException =
        new AtomicReference<Exception>();

    /**
     * Used to manage the put executor tasks.
     */
    private ExecutorService shardExecutor = null;

    /**
     * Used to manage the stream reader tasks.
     */
    private ExecutorService streamExecutor = null;

    public BulkPut(KVStoreImpl store,
                   BulkWriteOptions options,
                   List<EntryStream<T>> streams,
                   Logger logger) {

        this.logger = logger;

        this.store = store;
        topology = store.getTopology();
        serializer = store.getKeySerializer();

        this.options = options;
        @SuppressWarnings("unchecked")
        final EntryStream<T>[] array =
            streams.toArray(new EntryStream[streams.size()]);
        this.streams = array;

        readers = new ArrayList<StreamReader<T>>();

        final int nPartitions = topology.getPartitionMap().size();

        partitionThresholdBytes = computeThresholdBytes(nPartitions);

        @SuppressWarnings("unchecked")
        PartitionValues[] partitionValues =
            new BulkPut.PartitionValues[nPartitions + 1];
        pMap = partitionValues;
        for (int i = 0 ; i <= nPartitions ; i++) {
            pMap[i] = new PartitionValues(i);
        }
    }


    /**
     * Computes the threshold bytes that may be used to hold the KV pairs
     * associated with a partition. The larger the threshold, the larger the
     * potential batch size, and the more efficient the btree insert, since the
     * keys are likely to be more clustered and will impact fewer nodes.
     *
     * @param nPartitions the number of partitions associated with the store
     *
     * @return the threshold bytes associated with each partition
     */
    private long computeThresholdBytes(final int nPartitions) {
        final long maxHeapBytes = Runtime.getRuntime().maxMemory();

        if (maxHeapBytes == Long.MAX_VALUE) {
            final String msg =
                "Could not determine a max heap size. This is unusual. " +
                "Please specify the -Xmx argument to the jvm invocation " +
                "as a workaround";
            throw new IllegalArgumentException(msg);
        }
        final int bulkHeapPercent = options.getBulkHeapPercent();
        final long maxLoadHeapBytes =
            (maxHeapBytes * bulkHeapPercent) / 100;

        /*
         * Factor of two to allow for serialization of kv pairs from
         * map to batch request -- conservative but safe.
         */
        final long thresholdBytes = (maxLoadHeapBytes / 2) / nPartitions;

        if (thresholdBytes < partitionHeapMinBytes) {
            final long minHeapBytes =
                (((nPartitions * partitionHeapMinBytes) * 2) * 100) /
                bulkHeapPercent;
            logger.warning("Insufficient heap:" + maxHeapBytes + ". For best " +
                           "performance increase -Xmx on jvm invocation " +
                           "to a min of "  + (minHeapBytes / (1024 * 1024)) +
                           "mb");
        }

        final String fmt = "Buffer bytes per partition:%,d Max heap " +
        		"memory:%,d Bulk heap %% %,d";
        logger.info(String.format(fmt, thresholdBytes, maxHeapBytes,
                                  bulkHeapPercent));

        return thresholdBytes;
    }


    /**
     * Abstract method used to create a Row or KV reader
     *
     * @param streamId the internal unique stream id to be associated with the
     * user supplied stream
     * @param stream the user supplied stream
     *
     * @return the stream reader
     */
    public abstract StreamReader<T>
        createReader(int streamId, EntryStream<T> stream) ;

    /**
     * Abstract method used to convert Key/Value pair to T instance, it is used
     * to construct the input entry instance for EntryStream.keyExists(T entry).
     */
    protected abstract T convertToEntry(Key key, Value value);

    /**
     * Implements the bulk put operation.
     */
    public void execute()
        throws InterruptedException {

        shardExecutor = startShardExecutor();

        final ThreadFactory threadFactory =
            new KVThreadFactory("BulkStreamReader", logger);

        streamExecutor =
            Executors.newFixedThreadPool(options.getStreamParallelism(),
                                         threadFactory);
        int streamId = 0;

        ArrayList<Future<Integer>>
            futures = new ArrayList<Future<Integer>>(streams.length);
        for (EntryStream<T> s : streams) {
            final StreamReader<T> streamReader = createReader(++streamId, s);
            if (!futures.add(streamExecutor.submit(streamReader))) {
                throw new IllegalStateException
                    ("failed to add new future for stream:" + s.name());
            }
            readers.add(streamReader);
        }

        streamExecutor.shutdown();

        logProgress(streamExecutor);

        finishStreams(futures, statistics);

        flushPartitions();

        shutdownShardExecutor(shardExecutor);

        logger.log(Level.INFO, statistics.toString());

        if (terminateException.get() != null) {
            throw new FaultException(terminateException.get(), false);
        }
    }

    /**
     * Log progress at one minute intervals until all the streams have
     * reached EOF
     */
    private void logProgress(final ExecutorService readerExecutor)
        throws InterruptedException {

        final long startMs = System.currentTimeMillis();
        long prevTotalRead = 0;

        while (!readerExecutor.awaitTermination(1, TimeUnit.MINUTES)) {

            final String fmt = "Loading continues. %,d values read. " +
                "Throughput:%,d values/sec";
            final long totalRead = totalRead();
            final long throughput = (totalRead * 1000) /
                (System.currentTimeMillis() - startMs);
            logger.log((totalRead > prevTotalRead) ?
                Level.INFO : Level.WARNING,
                String.format(fmt, totalRead, throughput));
            prevTotalRead = totalRead;
        }
    }

    /**
     * Shutdown executor used to flush batches to shards. This method is only
     * invoked after all user supplied streams have reached EOF and all
     * partition batches have been flushed.
     */
    private void shutdownShardExecutor(final ExecutorService putExecutor)
        throws InterruptedException {

        final Set<ShardPutTask> rgTasks = new HashSet<ShardPutTask>();

        for (PartitionValues pv : pMap) {
            if (pv.partitionId == 0) {
                /* Ignore dummy partition id zero */
                continue;
            }

           rgTasks.add(pv.shardPutTask);
           pv.shardPutTask.add(partitionBatchEOF);
        }

        putExecutor.shutdown();

        while (!putExecutor.awaitTermination(1, TimeUnit.MINUTES)) {
            final String fmt = "Flushing puts";
            logger.info(fmt);
        }

        /* Collect statistics. */
        for (ShardPutTask rgp : rgTasks) {
            statistics.batchCount += rgp.batchCount;
            statistics.batchQueueUnderflow += rgp.batchQueueUnderflow;
            statistics.batchQueueOverflow += rgp.batchQueueOverflow;
            statistics.existingKeys += rgp.existingKeyCount;
            statistics.putCount += rgp.putCount;
        }
    }

    /**
     * Create the tasks that will write batches of partition entries to their
     * respective shard. The number of tasks per shard is defined by the
     * configuration parameter: perShardParallelism. There is at least one task
     * per shard to ensure that all shards are kept busy during the load.
     *
     * The partitions are divided amongst each shard to ensure that no two
     * tasks ever update the same partition and thus never conflict on locks.
     */
    private ExecutorService startShardExecutor() {

        final int perShardParallelism = options.getPerShardParallelism();

        final int numShardTasks =
            topology.getRepGroupMap().size() * perShardParallelism;

        final ExecutorService putExecutor =
            Executors.newFixedThreadPool(numShardTasks,
                                         new KVThreadFactory("RGWriter",
                                                             logger));

        final Map<RepGroupId, List<PartitionId>> map =
            TopologyUtil.getRGIdPartMap(topology);

        for (RepGroupId rgId : topology.getRepGroupIds()) {
            final List<PartitionId> list = map.get(rgId);

            /* Divide up the partitions amongst the tasks. */

            int perTaskPartitions =
                (list.size() + perShardParallelism - 1) / perShardParallelism;

            doneWithShard:
            for (int i = 0; i < perShardParallelism; i++) {
                final List<PartitionId> taskPartitions =
                    list.subList(i * perTaskPartitions,
                                 Math.min((i + 1) * perTaskPartitions,
                                          list.size()));

                if (taskPartitions.size() == 0) {
                    /* Excess threads */
                    break doneWithShard;
                }

                logger.info("Partitions:" +
                            Arrays.toString(taskPartitions.toArray()) +
                            " assigned to RG task");
                final ShardPutTask putTask =
                    new ShardPutTask(rgId, taskPartitions.size());

                for (PartitionId pid : taskPartitions) {
                    PartitionValues pv = pMap[pid.getPartitionId()];
                    pv.setShardPutTask(putTask);
                }

                putExecutor.submit(putTask);
            }
        }

        return putExecutor;
    }

    /*
     * Terminates the whole bulk put operation and record the exception.
     */
    private void terminateWithException(Exception exception) {

        if (!terminateException.compareAndSet(null, exception)) {
            /*
             * Multiple exceptions. Ignore subsequent ones since we are already
             * shutting down.
             */
            return ;
        }

        List<Runnable> unfinishedBusiness = streamExecutor.shutdownNow();
        if (!unfinishedBusiness.isEmpty()) {
            final int nRemainingTasks = unfinishedBusiness.size();
            logger.log(Level.FINE,
                       "Bulk put reader stream executor didn't shutdown " +
                       "cleanly. {0} tasks remaining.", nRemainingTasks);
        }

        unfinishedBusiness = shardExecutor.shutdownNow();
        if (!unfinishedBusiness.isEmpty()) {
            final int nRemainingTasks = unfinishedBusiness.size();
            logger.log(Level.FINE,
                       "Bulk put shard executor didn't shutdown cleanly. "+
                       "{0} tasks remaining.", nRemainingTasks);
        }
    }

    /**
     * Returns true if the current bulk put operation is terminated.
     */
    private boolean isTermiated() {
        return terminateException.get() != null;
    }

    /**
     * Canonical PartitionBatch object to signify EOF in the partition batch
     * queue.
     */
    private static PartitionBatch partitionBatchEOF = new PartitionBatch();

    /**
     * Used to hold a sorted list of KV pairs. The sorted list ensures locality
     * of reference during insertion on the server.
     */
    private static class PartitionBatch {
        final PartitionId pid;
        final List<KVPair> kvPairs;

        /* The collection of table ids for the batch entries */
        /*
         * CRC: revisit to see if we actually need these once the dust has
         * settled around SR 24670
         */
        final Set<Long> tableIds;

        /* The map of stream ids and corresponding entry count */
        final Map<Integer, Integer> perStreamCount;

        PartitionBatch() {
            this(null, null, null, null);
        }

        PartitionBatch(PartitionId pid,
                       List<KVPair> kvPairs,
                       Set<Long> tableIds,
                       Map<Integer, Integer> perStreamCount) {
            super();
            this.pid = pid;
            this.kvPairs = kvPairs;
            this.tableIds = tableIds;
            this.perStreamCount = perStreamCount;
        }

        public Integer[] getStreamIds() {
            final Set<Integer> streamIds = perStreamCount.keySet();
            return streamIds.toArray(new Integer[streamIds.size()]);
        }

        public int getStreamEntryCount(int streamId) {
            return perStreamCount.get(streamId);
        }

        public long[] getTableIds() {
            if (tableIds == null) {
                return null;
            }
            long[] tids = new long[tableIds.size()];
            int i = 0;
            for (Long id : tableIds) {
                tids[i++] = id.longValue();
            }
            return tids;
        }
    }

    /**
     * The task used to write a PartitionBatch to its shard.
     */
    public class ShardPutTask implements Runnable {

        /**
         * The shard associated with this task
         */
        final RepGroupId rgId;

        /**
         * The total number of puts completed by this task
         */
        public long putCount;

        /**
         *  The queue of batches to be processed by this task.
         */
        private final ArrayBlockingQueue<PartitionBatch> queuedKVPairs;

        /**
         * The number of batches processed by this task.
         */
        private long batchCount = 0 ;

        /**
         * The number of times this task was blocked because it did not have
         * a partition batch to write. Large numbers of queue underflows
         * indicate that the user input streams are not providing data fast
         * enough and increasing stream parallelism could help.
         */
        private long batchQueueUnderflow = 0 ;

        /**
         * The number of times a batch could not be inserted because there
         * was no space in the queue. Large numbers of queue overflows
         * indicate that performance could benefit from increased shard
         * parallelism.
         */
        private long batchQueueOverflow = 0 ;

        /**
         * The number of keys that were already present in the store.
         */
        private long existingKeyCount;

        public ShardPutTask(RepGroupId rgId,
                            int numTaskPartitions) {
            this.rgId = rgId;

            queuedKVPairs =
                new ArrayBlockingQueue<PartitionBatch>(numTaskPartitions * 2) ;
        }

        void add(PartitionBatch partBatch)
            throws InterruptedException {

            if (!queuedKVPairs.offer(partBatch)) {
                batchQueueOverflow++;
                queuedKVPairs.put(partBatch);
            }
        }

        @Override
        public void run() {
            final String sfmt = "Starting RG thread. Shard:%s";
            Integer[] streamIds = null;
            logger.info(String.format(sfmt, rgId));
            try {
                while (true) {
                    PartitionBatch pbatch = queuedKVPairs.poll();
                    if (pbatch == null) {
                        batchQueueUnderflow++;
                        pbatch = queuedKVPairs.take();
                    }

                    if (pbatch == partitionBatchEOF) {
                        return;
                    }

                    streamIds = pbatch.getStreamIds();
                    batchCount++;

                    try {
                        final List<Integer> existing =
                            store.putBatch(pbatch.pid,
                                           pbatch.kvPairs,
                                           pbatch.getTableIds(),
                                           options.getDurability(),
                                           options.getTimeout(),
                                           options.getTimeoutUnit());
                        putCount += pbatch.kvPairs.size();

                        for (int pos : existing) {
                            final KVPair kvPair = pbatch.kvPairs.get(pos);
                            final T entry = convertKVPairToEntry(kvPair);
                            streams[kvPair.getStreamId() - 1].keyExists(entry);
                            existingKeyCount++;
                            logger.info("Existing key at sub-batch pos:" + pos);
                        }
                    } catch (RuntimeException re) {
                        logger.info(Thread.currentThread() + " caught " + re);
                        handleRuntimeException(pbatch, re);
                    }

                    tallyEntryCount(streamIds, pbatch);
                }
            } catch (InterruptedException ie) {
                logger.info(Thread.currentThread() + " caught " + ie);
                terminateWithException(new RuntimeException(ie));
            } finally {
                final String fmt = "Exiting RG thread. Shard:%s";
                logger.info(String.format(fmt, rgId));
            }
        }
    }

    /**
     * Tally the entry count for the related entry streams in this batch, if
     * the last entry of stream is put to the store, invoke the
     * EntryStream.completed() method.
     */
    private void tallyEntryCount(final Integer[] streamIds,
                                 final PartitionBatch pbatch) {

        for (Integer streamId : streamIds) {
            final int count = pbatch.getStreamEntryCount(streamId);
            final StreamReader<T> reader = readers.get(streamId - 1);
            reader.tallyOpCount(count);
            if (reader.isDone()) {
                reader.getEntryStream().completed();
            }
        }
    }

    /**
     * Invoke EntryStream.catchException() method for all the related entry
     * streams, terminates the whole bulk put operation if
     * EntryStream.catchException() thrown exception.
     */
    private void handleRuntimeException(final PartitionBatch pbatch,
                                        RuntimeException re) {
        for (KVPair kv : pbatch.kvPairs) {
            final int streamId = kv.getStreamId();
            final T entry = convertKVPairToEntry(kv);
            try {
                streams[streamId - 1].catchException(re, entry);
            } catch (Exception ex) {
                terminateWithException(ex);
                break;
            }
        }
    }

    /**
     * Convert a KVPair object to an entry object.
     */
    private T convertKVPairToEntry(final KVPair kv) {
        final Key key = serializer.fromByteArray(kv.getKey());
        final Value value = Value.fromByteArray(kv.getValue());
        return convertToEntry(key, value);
    }

    /**
     * Flush all residual values that were queued at their partitions to their
     * respective shards.
     */
    private void flushPartitions()
        throws InterruptedException {

        for (PartitionValues pv : pMap) {
           pv.flush(true);
        }

        logger.info("Flushed all partitions");
    }

    /**
     * Wait for all futures queued to read streams to finish and accumulate
     * read counts.
     */
    private void finishStreams(ArrayList<Future<Integer>> futures,
                               AggregateStatistics putResult)
        throws InterruptedException  {

        for (Future<Integer> f : futures) {
            if (isTermiated()) {
                f.cancel(true);
                continue;
            }
            try {
                int readCount = f.get();
                putResult.aggregate(readCount);
            } catch (ExecutionException e) {
                final Throwable t = e.getCause();

                if (t instanceof RuntimeException) {
                    throw (RuntimeException) t;
                }

                throw new IllegalStateException(t);
            }
        }
    }

    /**
     * Represents the aggregate statistics across all streams.
     */
    private static class AggregateStatistics {

        private long batchCount;
        private long batchQueueUnderflow ;
        private long batchQueueOverflow;

        /**
         * The total number of entries read from all the streams
         * supplied to the operation.
         */
        private long readCount ;

        /**
         * The total number of entries actually inserted into the store
         * as a result of the operation. This number is typically
         * equal to the number returned by {@link #entriesRead} but may be less
         * if entries supplied by the stream have primary keys that are already
         * present in the store. Or if reading from a stream was abandoned due
         * to an exception.
         */
        private long putCount ;

        /**
         * The number of entries that were rejected because there was
         * already an entry, with the same primary key in the store.
         */
        private long existingKeys ;

        public void aggregate(int entriesRead) {
            readCount += entriesRead;
        }

        @Override
        public String toString() {
            final String fmt =
                "%,d rows read, %,d inserted, %,d pre-existing. " +
                "%,d batches; %,d batch queue underflows; " +
                "%,d batch queue overflows;"  +
                 "%,d av batch size";

            return String.format(fmt, readCount, putCount, existingKeys,
                                 batchCount,
                                 batchQueueUnderflow,
                                 batchQueueOverflow,
                                 ((batchCount > 0) ?
                                     (putCount / batchCount) : 0));
        }
    }


    /**
     * Total entries read from all streams
     */
    private long totalRead() {
        long totalRead = 0;
        for (StreamReader<T> reader: readers) {
            totalRead += reader.getReadCount();
        }
        return totalRead;
    }

    /**
     * Dedicated task used to read a specific row or KV stream
     *
     * @param <E> the entry type: Row or a KV pair
     */
    public abstract class StreamReader<E> implements Callable<Integer> {

        /**
         * The internal stream id
         */
        private final int streamId;

        /**
         * The stream being read
         */
        private final EntryStream<E> entryStream;

        /**
         * The number of records read by this stream reader.
         */
        private volatile int readCount = 0;

        /**
         * The flag to indicate if there are no more elements to read.
         */
        private boolean noMoreElement = false;

        /**
         * The number of records put to store.
         */
        private final AtomicLong putCount;

        public StreamReader(int streamId, EntryStream<E> entryStream) {
            super();
            this.streamId = streamId;
            this.entryStream = entryStream;
            putCount = new AtomicLong();
        }

        @Override
        public Integer call() throws Exception {
           final String sfmt = "Started stream reader for %s(%d)";
           logger.info(String.format(sfmt, entryStream.name(), streamId));

           try {

               for (E e = entryStream.getNext();
                    (e != null) ;
                    e = entryStream.getNext()) {

                   readCount++;

                   final Key pk = getKey(e);
                   final Value value = getValue(e);
                   final long tableId = getTableId(e);
                   final byte[] keyBytes = serializer.toByteArray(pk);
                   final PartitionId pid = topology.getPartitionId(keyBytes);

                   pMap[pid.getPartitionId()].put(keyBytes,
                                                  value.toByteArray(),
                                                  streamId,
                                                  tableId);
               }
               noMoreElement = true;
               /*
                * Invoke the EntryStream.completed() if no element read
                * from the stream
                */
               if (readCount == 0) {
                   entryStream.completed();
               }
           } catch (RuntimeException re) {
               terminateWithException(re);
           } finally {
               final String fmt = "Finished stream reader for %s(%d)";
               logger.info(String.format(fmt, entryStream.name(), streamId));
           }
           return readCount;
        }

        EntryStream<E> getEntryStream() {
            return entryStream;
        }

        int getReadCount() {
            return readCount;
        }

        void tallyOpCount(int count) {
            putCount.addAndGet(count);
        }

        /**
         * Return true if all elements are read and write to the store.
         */
        boolean isDone() {
            return noMoreElement && (putCount.get() == readCount);
        }

        /**
         * Abstract methods used to abstract how the keys and values are
         * obtained.
         */
        protected abstract Key getKey(E entry);

        protected abstract Value getValue(E entry);

        protected long getTableId(@SuppressWarnings("unused") E entry) {
            return 0;
        }
    }

    /**
     * The values associated with a specific partition.
     */
    private class PartitionValues {

        /**
         * The partition associated with the values.
         */
        private final int partitionId;

        /**
         * The task designated to write this partition's values to its shard.
         */
        private ShardPutTask shardPutTask;

        /**
         * The number of entries that were actually inserted into the partition.
         */
        private long putCount = 0;

        /**
         * Holds the sorted values that are waiting to be written to the shard.
         * Tried ConcurrentSkipListMap to eliminate use of synchronized methods
         * but it resulted in lower perf on Nashua machines.
         */
        private final SortedMap<byte[], WrappedValue> kvPairs =
            new TreeMap<byte[], WrappedValue>(KEY_BYTES_COMPARATOR);

        /**
         * The total number of key/value bytes stored in kvPairs. It's
         * compared against the partition threshold to help determine when to
         * flush kv pairs to the partition.
         */
        private long treeBytes = 0;

        public void setShardPutTask(ShardPutTask rgPutThread) {
            this.shardPutTask = rgPutThread;
        }

        PartitionValues(int pid) {
            super();
            this.partitionId = pid;
        }

        synchronized void put(byte[] key, byte[] value,
                              int streamId, long tableId)
            throws InterruptedException {

            final WrappedValue wv = new WrappedValue(value, streamId, tableId);
            kvPairs.put(key, wv);
            treeBytes += (key.length + wv.getBytesSize());
            flush(false);
        }

        /**
         * Flushes values in the kvPairs tree to the shard if needed.
         *
         * A flush is typically done if the size of the storage occupied by
         * kvPairs exceeds the threshold number of bytes associated with the
         * partition.
         *
         * @param force if true the partition is flushed even if the threshold
         * has not been reached
         *
         */
        private void flush(boolean force)
            throws InterruptedException {

            final int maxRequestSize = 1024 * 1024;

            final String fmt =
                "Queued Partition %d flushed. Batch size %,d; Total:%,d;" +
                    " Tree bytes:%,d; request size:%,d" ;

            while ((force && kvPairs.size() > 0) ||
                   (treeBytes >  partitionThresholdBytes)) {

                int putBatchCount = 0;
                int requestSize = 0;
                final List<KVPair> le = new ArrayList<KVPair>();
                final Map<Integer, Integer> streamIdCountMap =
                    new HashMap<Integer, Integer>();
                final Set<Long> tableIds = new HashSet<Long>();
                synchronized (this) {

                    for (Iterator<Entry<byte[], WrappedValue>> iter =
                         kvPairs.entrySet().iterator();
                         iter.hasNext();) {

                        final Entry<byte[], WrappedValue> e = iter.next();
                        iter.remove();

                        final byte[] key = e.getKey();
                        final WrappedValue wv = e.getValue();
                        final byte[] value = wv.getValue();
                        final int streamId = wv.getStreamId();
                        final long tableId = wv.getTableId();
                        if (tableId != 0) {
                            tableIds.add(wv.getTableId());
                        }

                        putBatchCount++;
                        final int kvBytes = key.length + wv.getBytesSize();
                        treeBytes -= kvBytes;
                        requestSize += kvBytes;

                        le.add(new KVPair(key, value, streamId));
                        tallyEntryCount(streamIdCountMap, streamId);
                        if (requestSize > maxRequestSize) {
                            break;
                        }
                    }

                    putCount += putBatchCount;
                }

                /* Can block, do it outside sync block */
                shardPutTask.add(
                    new PartitionBatch(new PartitionId(partitionId), le,
                                       (tableIds.isEmpty() ? null : tableIds),
                                       streamIdCountMap));

                logger.fine(String.format(fmt, partitionId,
                                          putBatchCount, putCount,
                                          treeBytes, requestSize));
            }
        }

        /**
         * Tally the entry count for each stream in the current batch.
         */
        private void tallyEntryCount(final Map<Integer, Integer> streamCountMap,
                                     final int streamId) {
            if (streamCountMap.containsKey(streamId)) {
                final int count = streamCountMap.get(streamId) + 1;
                streamCountMap.put(streamId, count);
            } else {
                streamCountMap.put(streamId, 1);
            }
        }
    }

    /**
     * A class represents a value information that includes value bytes, and
     * its corresponding streamId and tableId.
     */
    private static class WrappedValue {

        private final byte[] value;
        private final int streamId;
        private final long tableId;

        WrappedValue(byte[] value, int streamId, long tableId) {
            this.value = value;
            this.streamId = streamId;
            this.tableId = tableId;
        }

        int getStreamId() {
            return streamId;
        }

        long getTableId() {
            return tableId;
        }

        byte[] getValue() {
            return value;
        }

        /*
         * Returns the bytes size of a WrappedValue object, currently it is
         * calculated as the total size of its 3 members: value, streamId,
         * tableId.
         *
         * TODO: overhead to add?
         */
        int getBytesSize() {
            return value.length + 4 + 8;
        }
    }

    /**
     * A simple "struct" used to hold a key/value pair
     */
    static public class KVPair {
        final byte[] key;
        final byte[] value;
        final int streamId;

        public KVPair(byte[] key, byte[] value) {
            this(key, value, -1);
        }

        public KVPair(byte[] key, byte[] value, int streamId) {
            super();
            this.key = key;
            this.value = value;
            this.streamId = streamId;
        }

        public byte[] getKey() {
            return key;
        }

        public byte[] getValue() {
            return value;
        }

        public int getStreamId() {
            return streamId;
        }
    }
}
