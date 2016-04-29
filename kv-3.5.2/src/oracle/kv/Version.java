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

package oracle.kv;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutput;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.nio.ByteBuffer;
import java.util.UUID;

import com.sleepycat.je.utilint.DbLsn;
import com.sleepycat.je.utilint.VLSN;
import oracle.kv.impl.topo.RepNodeId;
import oracle.kv.impl.topo.ResourceId;
import oracle.kv.impl.util.FastExternalizable;
import oracle.kv.impl.util.SerialVersion;

/**
 * A Version refers to a specific version of a key-value pair.
 * <p>
 * When a key-value pair is initially inserted in the KV Store, and each time
 * it is updated, it is assigned a unique version token.  The version is always
 * returned by the put method, for example, {@link KVStore#put put}, and is
 * also returned by get methods, for example, {@link KVStore#get get}.  The
 * version is important for two reasons:
 * <ol>
 *   <li>
 *   When an update or delete is to be performed, it may be important to only
 *   perform the update or delete if the last known value has not changed.  For
 *   example, if an integer field in a previously known value is to be
 *   incremented, it is important that the previous value has not changed in
 *   the KV Store since it was obtained by the client.  This can be guaranteed
 *   by passing the version of the previously known value to the {@link
 *   KVStore#putIfVersion putIfVersion} or {@link KVStore#deleteIfVersion
 *   deleteIfVersion} method.  If the version specified does not match the
 *   current version of the value in the KV Store, these methods will not
 *   perform the update or delete operation and will return an indication of
 *   failure.  Optionally, they will also return the current version and/or
 *   value so the client can retry the operation or take a different action.
 *   </li>
 *   <li>
 *   When a client reads a value that was previously written, it may be
 *   important to ensure that the KV Store node servicing the read operation
 *   has been updated with the information previously written.  This can be
 *   accomplished by passing the version of the previously written value as
 *   a {@link Consistency} parameter to the read operation, for example, {@link
 *   KVStore#get get}.  See {@link Consistency.Version} for more information.
 *   </li>
 * </ol>
 * </p>
 * <p>
 * It is important to be aware that the system may infrequently assign a new
 * Version to a key-value pair, for example, when migrating data for better
 * resource usage.  Therefore, when using the {@link KVStore#putIfVersion
 * putIfVersion} or {@link KVStore#deleteIfVersion deleteIfVersion} methods,
 * one cannot assume that the Version will remain constant until it is changed
 * by the application.
 * </p>
 */
public class Version implements FastExternalizable, Serializable {

    private static final long serialVersionUID = 1;
    final static short MAGIC = (short)0x04db;
    /*
     * The UUID associated with the replicated environment.
     */
    private final UUID repGroupUuid;
    private final long repGroupVlsn;
    private final RepNodeId repNodeId;
    private final long repNodeLsn;

    /**
     * For internal use only.
     * @hidden
     *
     * Creates a Version with a logical VLSN but without a physical LSN.
     */
    public Version(UUID repGroupUuid, long repGroupVlsn) {
        assert repGroupUuid != null;
        assert repGroupVlsn > 0;

        this.repGroupUuid = repGroupUuid;
        this.repGroupVlsn = repGroupVlsn;
        this.repNodeId = null;
        this.repNodeLsn = 0;
    }

    /**
     * For internal use only.
     * @hidden
     *
     * Creates a Version with a logical VLSN and physical LSN.
     */
    public Version(UUID repGroupUuid,
                   long repGroupVlsn,
                   RepNodeId repNodeId,
                   long repNodeLsn) {
        assert repGroupUuid != null;
        assert repGroupVlsn > 0 : repGroupVlsn;
        assert repNodeId != null;
        assert repNodeLsn != 0 && repNodeLsn != DbLsn.NULL_LSN : repNodeLsn;

        this.repGroupUuid = repGroupUuid;
        this.repGroupVlsn = repGroupVlsn;
        this.repNodeId = repNodeId;
        this.repNodeLsn = repNodeLsn;
    }

    /**
     * For internal use only.
     * @hidden
     *
     * FastExternalizable constructor.
     */
    public Version(ObjectInput in, short serialVersion)
        throws IOException {

        final long mostSig = in.readLong();
        final long leastSig = in.readLong();
        repGroupUuid = new UUID(mostSig, leastSig);
        repGroupVlsn = in.readLong();

        if (in.read() == 0) {
            repNodeId = null;
            repNodeLsn = 0;
            return;
        }

        /*
         * The serialized form supports ResourceIds of different subclasses,
         * but here we only allow a RepNodeId.
         */
        repNodeId = (RepNodeId) ResourceId.readFastExternal(in, serialVersion);

        repNodeLsn = in.readLong();
    }

    /**
     * For internal use only.
     * @hidden
     *
     * FastExternalizable writer.
     */
    @Override
    public void writeFastExternal(ObjectOutput out, short serialVersion)
        throws IOException {

        out.writeLong(repGroupUuid.getMostSignificantBits());
        out.writeLong(repGroupUuid.getLeastSignificantBits());
        out.writeLong(repGroupVlsn);
        if (repNodeId == null) {
            out.write(0);
            return;
        }
        out.write(1);
        repNodeId.writeFastExternal(out, serialVersion);
        out.writeLong(repNodeLsn);
    }

    /**
     * Returns this Version as a serialized byte array, such that {@link
     * #fromByteArray} may be used to reconstitute the Version.
     */
    public byte[] toByteArray() {
        /*
         * This format is compatible with that created by ObjectOutputStream.
         * The array has 2 components, the header information required by
         * ObjectOutputStream and the actual payload.
         *
         * The total size is 33 or 50 bytes, depending on repNodeId.  The
         * first 6 bytes is the ObjectOutputStream header and the remaining 
         * bytes are the serialized Version.
         */
        final int headerSize = 6;
        /* totalSize - headerSize */
        final byte payloadSize = (repNodeId == null ? (byte)27 : (byte)44);
        
        ByteBuffer bb = ByteBuffer.allocate(headerSize + payloadSize);

        bb.putShort(ObjectOutputStream.STREAM_MAGIC);
        bb.putShort(ObjectOutputStream.STREAM_VERSION);
        bb.put(ObjectOutputStream.TC_BLOCKDATA);
        /* remaining length */
        bb.put(payloadSize);
        bb.putShort(SerialVersion.CURRENT);
        bb.putLong(repGroupUuid.getMostSignificantBits());
        bb.putLong(repGroupUuid.getLeastSignificantBits());
        bb.putLong(repGroupVlsn);

        if (repNodeId == null) {
            bb.put((byte)0);
        } else {
            bb.put((byte)1);
            bb.put((byte)(repNodeId.getType().ordinal()));
            bb.putInt(repNodeId.getGroupId());
            bb.putInt(repNodeId.getNodeNum());
            bb.putLong(repNodeLsn);
        }

        return bb.array();
    }

    /**
     * Deserializes the given bytes that were returned earlier by {@link
     * #toByteArray} and returns the resulting Version.
     */
    public static Version fromByteArray(byte[] keyBytes) {

        final ByteArrayInputStream bais = new ByteArrayInputStream(keyBytes);
        try {
            final ObjectInputStream ois = new ObjectInputStream(bais);

            final short serialVersion = ois.readShort();

            return new Version(ois, serialVersion);

        } catch (IOException e) {
            /* Should never happen. */
            throw new FaultException(e, false /*isRemote*/);
        }
    }

    /**
     * For internal use only.
     * @hidden
     */
    public UUID getRepGroupUUID() {
        return repGroupUuid;
    }

    /**
     * For internal use only.
     * @hidden
     */
    public long getVLSN() {
        return repGroupVlsn;
    }

    /**
     * This method is hidden and deprecated, and should not be used.  It will
     * probably be removed entirely in a future release.
     *
     * <p>This method returns the VLSN associated with a version, which only
     * identifies the version relative to a particular shard.  If applications
     * need a way to compare versions to determine which one is newer, we could
     * provide a method that compares VLSNs but throws IllegalArgumentException
     * if the shards differ.  [#23526]
     *
     * @hidden
     * @deprecated
     */
    @Deprecated
    public long getVersion() {
        return repGroupVlsn;
    }

    @Override
    public boolean equals(Object other) {
        if (!(other instanceof Version)) {
            return false;
        }
        final Version o = (Version) other;
        return repGroupVlsn == o.repGroupVlsn &&
               repGroupUuid.equals(o.repGroupUuid);
    }

    @Override
    public int hashCode() {
        return repGroupUuid.hashCode() + (int) repGroupVlsn;
    }

    @Override
    public String toString() {
        return "<Version repGroupUuid=" + repGroupUuid +
               " vlsn=" + (new VLSN(repGroupVlsn)).toString() +
               " repNodeId=" + repNodeId +
               " lsn=" + DbLsn.getNoFormatString(repNodeLsn) + '>';
    }

    /**
     * For internal use only.
     * @hidden
     *
     * Returns true if the VLSN of this Version is equal to the given VLSN,
     * within the implied context of a replication group.  If false is
     * returned, it is certain that the versions are unequal.
     */
    public boolean sameLogicalVersion(long otherVlsn) {
        return repGroupVlsn == otherVlsn;
    }

    /**
     * For internal use only.
     * @hidden
     *
     * Returns true if the node Id and LSN of this Version are equal to the
     * given node Id and LSN, within the implied context of a replication
     * group.  If false is returned, then it is not known whether the versions
     * are equal and sameLogicalVersion must be called to determine equality.
     */
    public boolean samePhysicalVersion(RepNodeId otherNodeId,
                                       long otherNodeLsn) {
        return repNodeLsn == otherNodeLsn &&
               repNodeId != null &&
               repNodeId.equals(otherNodeId);
    }
}
