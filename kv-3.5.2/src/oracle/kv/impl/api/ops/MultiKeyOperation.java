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

package oracle.kv.impl.api.ops;

import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.sleepycat.je.DatabaseEntry;

import oracle.kv.Depth;
import oracle.kv.KeyRange;
import oracle.kv.impl.api.ops.OperationHandler.KVAuthorizer;
import oracle.kv.impl.api.ops.InternalOperation.Keyspace.KeyAccessChecker;
import oracle.kv.impl.api.ops.InternalOperation.Keyspace.KeyspaceType;
import oracle.kv.impl.api.ops.InternalOperation.PrivilegedTableAccessor;
import oracle.kv.impl.security.ExecutionContext;
import oracle.kv.impl.security.KVStorePrivilege;
import oracle.kv.impl.security.SystemPrivilege;
import oracle.kv.impl.util.UserDataControl;

/**
 * A multi-key operation has a parent key, optional KeyRange and depth.
 */
abstract class MultiKeyOperation extends InternalOperation
    implements PrivilegedTableAccessor {

    private static final KVAuthorizer UNIVERSAL_AUTHORIZER =
        new KVAuthorizer() {
            @Override
            public boolean allowAccess(DatabaseEntry keyEntry) {
                return true;
            }

            @Override
            public boolean allowFullAccess() {
                return true;
            }
        };

    /**
     * The parent key, or null.
     */
    private final byte[] parentKey;

    /**
     * Sub-key range of traversal, or null.
     */
    private final KeyRange subRange;

    /**
     * Depth of traversal, always non-null.
     */
    private final Depth depth;

    /**
     * Constructs a multi-key operation.
     *
     * For subclasses, allows passing OpCode.
     */
    MultiKeyOperation(OpCode opCode,
                      byte[] parentKey,
                      KeyRange subRange,
                      Depth depth) {
        super(opCode);
        this.parentKey = parentKey;
        this.subRange = subRange;
        this.depth = depth;
    }

    /**
     * FastExternalizable constructor.  Must call superclass constructor first
     * to read common elements.
     *
     * For subclasses, allows passing OpCode.
     */
    MultiKeyOperation(OpCode opCode, ObjectInput in, short serialVersion)
        throws IOException {

        super(opCode, in, serialVersion);

        final int keyLen = in.readShort();
        if (keyLen < 0) {
            parentKey = null;
        } else {
            parentKey = new byte[keyLen];
            in.readFully(parentKey);
        }

        if (in.read() == 0) {
            subRange = null;
        } else {
            subRange = new KeyRange(in, serialVersion);
        }

        final int depthOrdinal = in.readByte();
        if (depthOrdinal == -1) {
            depth = null;
        } else {
            depth = Depth.getDepth(depthOrdinal);
        }
    }

    /**
     * FastExternalizable writer.  Must call superclass method first to write
     * common elements.
     */
    @Override
    public void writeFastExternal(ObjectOutput out, short serialVersion)
        throws IOException {

        super.writeFastExternal(out, serialVersion);

        if (parentKey == null) {
            out.writeShort(-1);
        } else {
            out.writeShort(parentKey.length);
            out.write(parentKey);
        }

        if (subRange == null) {
            out.write(0);
        } else {
            out.write(1);
            subRange.writeFastExternal(out, serialVersion);
        }

        if (depth == null) {
            out.writeByte(-1);
        } else {
            out.writeByte(depth.ordinal());
        }
    }

    /**
     * When the parent key is null or too short to determine the keyspace it
     * may access, returns a KVAuthorizer instance that will determine on a
     * per-KV entry basis whether entries are visible to the user. This
     * implements a policy that allows callers to iterate over the store
     * without generating errors if they come across something that they aren't
     * allowed access to.
     */
    KVAuthorizer checkPermission(OperationHandler operationHandler) {
        return checkPermission(operationHandler, parentKey);
    }

    KVAuthorizer checkPermission(OperationHandler operationHandler,
                                 byte[] key) {

        /* If security is not enabled, returns a fully accessible checker */
        if (ExecutionContext.getCurrent() == null) {
            return UNIVERSAL_AUTHORIZER;
        }

        final Set<KeyAccessChecker> checkers = new HashSet<KeyAccessChecker>();

        /* Checks if access server private keyspace is legal */
        if (!isInternalRequestor()) {
            if (key == null ||
                Keyspace.mayBePrivateAccess(key)) {
                checkers.add(Keyspace.privateKeyAccessChecker);
            }
        }

        /* Checks if access Avro schema keyspace is legal */
        if (!hasSchemaAccessPrivileges()) {
            if (key == null ||
                Keyspace.mayBeSchemaAccess(key)) {
                checkers.add(Keyspace.schemaKeyAccessChecker);
            }
        }

        /*
         * Checks if access general keyspace is legal.  When building the
         * execution context, the possibility of accessing exact private or
         * schema keyspace has been check. So at here, we know that we will
         * possibly access the general keyspace. If users do not have a full
         * access to general keyspace, the table access checker will be added
         * to allow table-specific permission checking.
         */
        if (!hasGeneralAccessPrivileges()) {
            checkers.add(new TableAccessChecker(operationHandler, this));
        }

        if (checkers.isEmpty()) {
            /*
             * Entries either cannot possible fall into the server private and
             * schema key space, or else we have an legal keyspace requestor,
             * so each access is guaranteed to be authorized.
             */
            return UNIVERSAL_AUTHORIZER;
        }

        /*
         * We have a user-level requestor, and either the parent key is null or
         * the parent key is not null, but is too short to be sure that no
         * illegal access will result, so entries could possibly fall into the
         * server private or schema key space.  Return an authorizer that will
         * check keys on each access.
         */
        return new KeyspaceAccessAuthorizer(checkers);
    }

    byte[] getParentKey() {
        return parentKey;
    }

    KeyRange getSubRange() {
        return subRange;
    }

    Depth getDepth() {
        return depth;
    }

    @Override
    public String toString() {
        return super.toString() +
            " parentKey: " + UserDataControl.displayKey(parentKey) +
            " subRange: " + UserDataControl.displayKeyRange(subRange) +
            " depth: " + depth;
    }

    /**
     * Checks whether the requestor has required privileges for schema access.
     */
    boolean hasSchemaAccessPrivileges() {
        final ExecutionContext currentContext = ExecutionContext.getCurrent();
        if (currentContext == null) {
            return true;
        }
        return currentContext.hasAllPrivileges(schemaAccessPrivileges());
    }

    /**
     * Checks whether the requestor has required privileges for general access.
     */
    boolean hasGeneralAccessPrivileges() {
        final ExecutionContext currentContext = ExecutionContext.getCurrent();
        if (currentContext == null) {
            return true;
        }
        return currentContext.hasAllPrivileges(generalAccessPrivileges());
    }

    @Override
    public List<? extends KVStorePrivilege> getRequiredPrivileges() {
        /*
         * If the parent key exactly falls in schema, private or general
         * keyspace, we can quickly check whether the user has the required
         * privilege.
         */
        if (parentKey != null) {
            final KeyspaceType keyspace = Keyspace.identifyKeyspace(parentKey);
            switch (keyspace) {
            case PRIVATE:
                return SystemPrivilege.internalPrivList;
            case SCHEMA:
                return schemaAccessPrivileges();
            case GENERAL:
                if (!Keyspace.mayBePrivateAccess(parentKey) &&
                    !Keyspace.mayBeSchemaAccess(parentKey)) {

                    /* Access exactly the general space, checks only the basic
                     * privilege for authentication, and let per-key checking
                     * be performed in each iteration.
                     */
                    return SystemPrivilege.usrviewPrivList;
                }
                break;
            default:
                throw new AssertionError();
            }
        }
        /*
         * The key is null or is too short to determine, we just check the
         * basic authentication here and defer the per-key checking to
         * iteration.
         */
        return SystemPrivilege.usrviewPrivList;
    }

    /**
     * Returns the required privileges for Avro schema keyspace access.
     */
    abstract List<? extends KVStorePrivilege> schemaAccessPrivileges();

    /**
     * Returns the required privileges for accessing the whole store keyspace
     * outside the schema and the server private keyspaces.
     */
    abstract List<? extends KVStorePrivilege> generalAccessPrivileges();

    /**
     * An authorizer for checking the permission of keyspace access of KVStore.
     * Currently, we implement the checking for the server-private keyspace,
     * the Avro schema keyspace, the table keyspace and non-table general
     * keyspace.
     */
    private static class KeyspaceAccessAuthorizer implements KVAuthorizer {

        private final Set<KeyAccessChecker> keyCheckers;

        /**
         * Constructs a KeyspaceAccessAuthorizer
         *
         * @param keyCheckers set of KeyAccessCheckers.
         */
        private KeyspaceAccessAuthorizer(Set<KeyAccessChecker> keyCheckers) {
            this.keyCheckers = keyCheckers;
        }

        @Override
        public boolean allowAccess(DatabaseEntry keyEntry) {
            final byte[] key = keyEntry.getData();
            for (final KeyAccessChecker checker : keyCheckers) {
                if (!checker.allowAccess(key)) {
                    return false;
                }
            }
            return true;
        }

        @Override
        public boolean allowFullAccess() {
            return false;
        }
    }
}
