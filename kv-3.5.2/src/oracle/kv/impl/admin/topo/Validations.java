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

package oracle.kv.impl.admin.topo;

import static oracle.kv.impl.util.ObjectUtil.checkNull;

import java.io.Serializable;
import java.util.List;
import java.util.Set;

import oracle.kv.impl.admin.TopologyCheck;
import oracle.kv.impl.admin.TopologyCheck.Remedy;
import oracle.kv.impl.admin.TopologyCheck.UpdateAdminParamsRemedy;
import oracle.kv.impl.admin.TopologyCheck.UpdateRNParamsRemedy;
import oracle.kv.impl.admin.VerifyConfiguration.Problem;
import oracle.kv.impl.topo.AdminId;
import oracle.kv.impl.topo.AdminType;
import oracle.kv.impl.topo.Datacenter;
import oracle.kv.impl.topo.DatacenterId;
import oracle.kv.impl.topo.DatacenterType;
import oracle.kv.impl.topo.RepGroupId;
import oracle.kv.impl.topo.RepNodeId;
import oracle.kv.impl.topo.ResourceId;
import oracle.kv.impl.topo.StorageNodeId;

import com.sleepycat.je.rep.NodeType;

/**
 * Classifications of topology problems. Some problems may have remedies. In
 * general, a problem due to inconsistencies in the configuration will have
 * a remedy and can be fixed by a repair plan. Problems requiring changes to
 * the topology should be handled through topology change plans.
 *
 * Note that each class must implement equals and hashcode, because unit tests
 * do some comparisons and manipulations of types of problems.
 */
public class Validations {

    /** A problem associated with failing to satisfy a rule. */
    public static abstract class RulesProblem
            implements Problem, Serializable {
        private static final long serialVersionUID = 1;

        RulesProblem() { }

        abstract boolean isViolation();

        /**
         * Returns the remedy for the problem if there is one, otherwise
         * null.
         *
         * @param topoCheck the topology checker in the remedy
         * @return the remedy or null
         */
        public Remedy getRemedy(TopologyCheck topoCheck) {
            return null;
        }
    }

    /** Base class for violations. */
    static abstract class Violation extends RulesProblem {
        private static final long serialVersionUID = 1;
        @Override
        public final boolean isViolation() {
            return true;
        }
    }

    /** Base class for warnings. */
    static abstract class Warning extends RulesProblem {
        private static final long serialVersionUID = 1;
        @Override
        public final boolean isViolation() {
            return false;
        }
    }

    /*
     * Violations
     */

    /**
     * This shard has fewer RNs in this datacenter than repFactor requires.
     */
    public static class InsufficientRNs extends Violation {
        private static final long serialVersionUID = 1L;
        private final DatacenterId dcId;
        private final int requiredRF;
        private final RepGroupId rgId;
        private final int numMissing;

        InsufficientRNs(DatacenterId dcId,
                        int requiredRF,
                        RepGroupId rgId,
                        int numMissing) {
            this.dcId = dcId;
            this.requiredRF = requiredRF;
            this.rgId = rgId;
            this.numMissing = numMissing;
        }

        @Override
        public ResourceId getResourceId() {
            return rgId;
        }

        @Override
        public String toString() {
            return rgId + " needs " + numMissing +
                " RNs to meet the required repFactor of " + requiredRF +
                " for " + dcId;
        }

        DatacenterId getDCId() {
            return dcId;
        }

        int getNumNeeded() {
            return numMissing;
        }

        RepGroupId getRGId() {
            return rgId;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + ((dcId == null) ? 0 : dcId.hashCode());
            result = prime * result + numMissing;
            result = prime * result + requiredRF;
            result = prime * result + ((rgId == null) ? 0 : rgId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof InsufficientRNs)) {
                return false;
            }
            InsufficientRNs other = (InsufficientRNs) obj;
            if (dcId == null) {
                if (other.dcId != null) {
                    return false;
                }
            } else if (!dcId.equals(other.dcId)) {
                return false;
            }
            if (numMissing != other.numMissing) {
                return false;
            }
            if (requiredRF != other.requiredRF) {
                return false;
            }
            if (rgId == null) {
                if (other.rgId != null) {
                    return false;
                }
            } else if (!rgId.equals(other.rgId)) {
                return false;
            }
            return true;
        }
    }

    /**
     * This SN hosts RNs that should not be on the same storage node.
     */
    public static class RNProximity extends Violation {
        private static final long serialVersionUID = 1L;
        private final StorageNodeId snId;
        private final RepGroupId rgId;
        private final List<RepNodeId> rnIds;

        RNProximity(StorageNodeId snId,
                    RepGroupId rgId,
                    List<RepNodeId> rnIds) {
            this.snId = snId;
            this.rgId = rgId;
            this.rnIds = rnIds;
        }

        @Override
        public ResourceId getResourceId() {
            return snId;
        }

        @Override
        public String toString() {
            return snId + " has too many RNs from the same shard(" + rgId +
                "): " + rnIds;
        }

        public StorageNodeId getSNId() {
            return snId;
        }

        public List<RepNodeId> getRNList() {
            return rnIds;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + ((rgId == null) ? 0 : rgId.hashCode());
            result = prime * result + ((rnIds == null) ? 0 : rnIds.hashCode());
            result = prime * result + ((snId == null) ? 0 : snId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof RNProximity)) {
                return false;
            }
            RNProximity other = (RNProximity) obj;
            if (rgId == null) {
                if (other.rgId != null) {
                    return false;
                }
            } else if (!rgId.equals(other.rgId)) {
                return false;
            }
            if (rnIds == null) {
                if (other.rnIds != null) {
                    return false;
                }
            } else if (!rnIds.equals(other.rnIds)) {
                return false;
            }
            if (snId == null) {
                if (other.snId != null) {
                    return false;
                }
            } else if (!snId.equals(other.snId)) {
                return false;
            }
            return true;
        }
    }

    /**
     * This SN hosts more RNs than its capacity setting.
     */
    public static class OverCapacity extends Violation {
        private static final long serialVersionUID = 1L;
        private final StorageNodeId snId;
        private final int rnCount;
        private final int capacityVal;

        OverCapacity(StorageNodeId snId, int rnCount, int capacityVal) {
            this.snId = snId;
            this.rnCount = rnCount;
            this.capacityVal = capacityVal;
        }

        @Override
        public ResourceId getResourceId() {
            return snId;
        }

        @Override
        public String toString() {
            return snId + " has " + rnCount +
                " repNodes and is over its capacity limit of " + capacityVal;
        }

        public int getExcess() {
            return rnCount - capacityVal;
        }

        public StorageNodeId getSNId() {
            return snId;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + capacityVal;
            result = prime * result + rnCount;
            result = prime * result + ((snId == null) ? 0 : snId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof OverCapacity)) {
                return false;
            }
            OverCapacity other = (OverCapacity) obj;
            if (capacityVal != other.capacityVal) {
                return false;
            }
            if (rnCount != other.rnCount) {
                return false;
            }
            if (snId == null) {
                if (other.snId != null) {
                    return false;
                }
            } else if (!snId.equals(other.snId)) {
                return false;
            }
            return true;
        }
    }

    /**
     * Indicates that the total heap used by the RNs hosted on this SN exceeds
     * the memory of that SN.
     */
    public static class RNHeapExceedsSNMemory extends Violation {

        private static final long serialVersionUID = 1L;
        private final StorageNodeId snId;
        private final long memoryMB;
        private final Set<RepNodeId> rnIds;
        private final long totalRNHeapMB;
        private final String rnMemList;

        public RNHeapExceedsSNMemory(StorageNodeId snId,
                int memoryMB,
                Set<RepNodeId> rnIds,
                long totalRNHeapMB,
                String rnMemList) {
            this.snId = snId;
            this.memoryMB = memoryMB;
            this.rnIds = rnIds;
            this.totalRNHeapMB = totalRNHeapMB;
            this.rnMemList = rnMemList;
        }

        @Override
        public String toString() {
            return snId + " is hosting " + (rnIds == null ? 0 : rnIds.size()) +
            " RNs whose combined heap of " + totalRNHeapMB +
            "MB exceeds the SN's limit of " + memoryMB +
            "MB. Resident RNs are " + rnMemList;
        }

        @Override
        public ResourceId getResourceId() {
            return snId;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + (int) (memoryMB ^ (memoryMB >>> 32));
            result = prime * result + ((rnIds == null) ? 0 : rnIds.hashCode());
            result = prime * result
                    + ((rnMemList == null) ? 0 : rnMemList.hashCode());
            result = prime * result + ((snId == null) ? 0 : snId.hashCode());
            result = prime * result
                    + (int) (totalRNHeapMB ^ (totalRNHeapMB >>> 32));
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof RNHeapExceedsSNMemory)) {
                return false;
            }
            RNHeapExceedsSNMemory other = (RNHeapExceedsSNMemory) obj;
            if (memoryMB != other.memoryMB) {
                return false;
            }
            if (rnIds == null) {
                if (other.rnIds != null) {
                    return false;
                }
            } else if (!rnIds.equals(other.rnIds)) {
                return false;
            }
            if (rnMemList == null) {
                if (other.rnMemList != null) {
                    return false;
                }
            } else if (!rnMemList.equals(other.rnMemList)) {
                return false;
            }
            if (snId == null) {
                if (other.snId != null) {
                    return false;
                }
            } else if (!snId.equals(other.snId)) {
                return false;
            }
            if (totalRNHeapMB != other.totalRNHeapMB) {
                return false;
            }
            return true;
        }
    }

    /**
     * A shard has no RNs in an SN in a primary datacenter in a store that has
     * secondary datacenters.
     */
    public static class NoPrimaryDC extends Violation {
        private static final long serialVersionUID = 1L;
        private final RepGroupId rgId;

        NoPrimaryDC(final RepGroupId rgId) {
            checkNull("rgId", rgId);
            this.rgId = rgId;
        }

        @Override
        public ResourceId getResourceId() {
            return rgId;
        }

        @Override
        public String toString() {
            return rgId + " has no RNs in a primary zone";
        }

        @Override
        public int hashCode() {
            final int prime = 83;
            int result = 1;
            result = prime * result + rgId.hashCode();
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof NoPrimaryDC)) {
                return false;
            }
            final NoPrimaryDC other = (NoPrimaryDC) obj;
            return rgId.equals(other.rgId);
        }
    }

    /**
     * A node with the wrong node type given the type of its zone.
     */
    public static class WrongNodeType extends Violation {
        private static final long serialVersionUID = 1L;
        private final RepNodeId rnId;
        private final NodeType nodeType;
        private final DatacenterId dcId;
        private final DatacenterType dcType;

        WrongNodeType(final RepNodeId rnId,
                      final NodeType nodeType,
                      final DatacenterId dcId,
                      final DatacenterType dcType) {
            checkNull("rnId", rnId);
            checkNull("nodeType", nodeType);
            checkNull("dcId", dcId);
            checkNull("dcType", dcType);
            if (nodeType == Datacenter.ServerUtil.getDefaultRepNodeType(
                    dcType)) {
                throw new IllegalArgumentException(
                    "The nodeType should not match the default node type" +
                    " for datacenter");
            }
            this.rnId = rnId;
            this.nodeType = nodeType;
            this.dcId = dcId;
            this.dcType = dcType;
        }

        @Override
        public ResourceId getResourceId() {
            return rnId;
        }

        @Override
        public Remedy getRemedy(TopologyCheck topoCheck) {
            return new UpdateRNParamsRemedy(topoCheck, rnId);
        }

        @Override
        public String toString() {
            return rnId + " has node type " + nodeType +
                ", but its zone, " + dcId +
                ", has type " + dcType +
                " and expects its replication nodes to have type " +
                Datacenter.ServerUtil.getDefaultRepNodeType(dcType);
        }

        @Override
        public int hashCode() {
            final int prime = 89;
            int result = 1;
            result = prime * result + rnId.hashCode();
            result = prime * result + nodeType.hashCode();
            result = prime * result + dcId.hashCode();
            result = prime * result + dcType.hashCode();
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof WrongNodeType)) {
                return false;
            }
            final WrongNodeType other = (WrongNodeType) obj;
            return rnId.equals(other.rnId) &&
                nodeType.equals(other.nodeType) &&
                dcId.equals(other.dcId) &&
                dcType.equals(other.dcType);
        }
    }

    /**
     * This zone is empty.
     */
    public static class EmptyZone extends Violation {
        private static final long serialVersionUID = 1L;
        private final DatacenterId dcId;

        EmptyZone(DatacenterId dcId) {
            checkNull("dcId", dcId);
            this.dcId = dcId;
        }

        @Override
        public ResourceId getResourceId() {
            return dcId;
        }

        DatacenterId getDCId() {
            return dcId;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + dcId.hashCode();
            return result;
        }

        @Override
        public String toString() {
            return "Zone " + dcId + " is empty";
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof EmptyZone)) {
                return false;
            }
            EmptyZone other = (EmptyZone) obj;
            return dcId.equals(other.dcId);
        }
    }

    /**
     * An Admin with the wrong node type given the type of its zone.
     */
    public static class WrongAdminType extends Violation {
        private static final long serialVersionUID = 1L;
        private final AdminId adminId;
        private final AdminType adminType;
        private final DatacenterId dcId;
        private final DatacenterType dcType;

        WrongAdminType(final AdminId adminId,
                       final AdminType adminType,
                       final DatacenterId dcId,
                       final DatacenterType dcType) {
            checkNull("adminId", adminId);
            checkNull("adminType", adminType);
            checkNull("dcId", dcId);
            checkNull("dcType", dcType);
            this.adminId = adminId;
            this.adminType = adminType;
            this.dcId = dcId;
            this.dcType = dcType;
        }

        @Override
        public ResourceId getResourceId() {
            return adminId;
        }

        @Override
        public String toString() {
            return adminId + " has node type " + adminType +
                   ", but its zone, " + dcId + ", has type " + dcType;
        }

        @Override
        public Remedy getRemedy(TopologyCheck topoCheck) {
            return new UpdateAdminParamsRemedy(topoCheck, adminId);
        }

        @Override
        public int hashCode() {
            final int prime = 89;
            int result = 1;
            result = prime * result + adminId.hashCode();
            result = prime * result + adminType.hashCode();
            result = prime * result + dcId.hashCode();
            result = prime * result + dcType.hashCode();
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof WrongAdminType)) {
                return false;
            }
            final WrongAdminType other = (WrongAdminType) obj;
            return adminId.equals(other.adminId) &&
                   adminType.equals(other.adminType) &&
                   dcId.equals(other.dcId) &&
                   dcType.equals(other.dcType);
        }
    }

    /**
     * This zone has fewer Admins than repFactor requires.
     */
    public static class InsufficientAdmins extends Violation {
        private static final long serialVersionUID = 1L;
        private final DatacenterId dcId;
        private final int requiredRF;
        private final int numMissing;

        InsufficientAdmins(DatacenterId dcId,
                           int requiredRF,
                           int numMissing) {
            checkNull("dcId", dcId);
            if (requiredRF < 1) {
                throw new IllegalArgumentException(
                        "The value of requiredRF must be > 0");
            }
            if (numMissing < 1) {
                throw new IllegalArgumentException(
                        "The value of numMissing must be > 0");
            }
            if (numMissing > requiredRF) {
                throw new IllegalArgumentException(
                        "The value of numMissing must be <= requiredRF");
            }
            this.dcId = dcId;
            this.requiredRF = requiredRF;
            this.numMissing = numMissing;
        }

        @Override
        public ResourceId getResourceId() {
            return dcId;
        }

        @Override
        public String toString() {
            return dcId + " needs " + numMissing +
                   " Admins to meet the required repFactor of " + requiredRF;
        }

        DatacenterId getDCId() {
            return dcId;
        }

        int getNumNeeded() {
            return numMissing;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + dcId.hashCode();
            result = prime * result + numMissing;
            result = prime * result + requiredRF;
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof InsufficientAdmins)) {
                return false;
            }
            final InsufficientAdmins other = (InsufficientAdmins) obj;
            return dcId.equals(other.dcId) &&
                   numMissing == other.numMissing &&
                   requiredRF == other.requiredRF;
        }
    }

    /**
     * Admin assigned to non-existent SN
     */
    public static class BadAdmin extends Violation {
        private static final long serialVersionUID = 1L;
        private final AdminId adminId;
        private final StorageNodeId snId;
        private final String desc;

        public BadAdmin(AdminId adminId, StorageNodeId snId) {
            checkNull("adminId", adminId);
            checkNull("snId", snId);
            this.adminId = adminId;
            this.snId = snId;
            desc = adminId + " assigned to " + snId + " but storage " +
                   "node not found";
        }

        @Override
        public ResourceId getResourceId() {
            return adminId;
        }

        @Override
        public String toString() {
            return desc;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + desc.hashCode();
            result = prime * result + adminId.hashCode();
            result = prime * result + snId.hashCode();
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof BadAdmin)) {
                return false;
            }
            BadAdmin other = (BadAdmin) obj;
            return desc.equals(other.desc) &&
                   adminId.equals(other.adminId) &&
                   snId.equals(other.snId);
        }
    }

    /*
     * WARNINGS
     */

    /**
     * This shard has more RNs in this datacenter than repFactor requires.
     */
    public static class ExcessRNs extends Warning {
        private static final long serialVersionUID = 1L;
        private final DatacenterId dcId;
        private final int requiredRF;
        private final RepGroupId rgId;
        private final int numExcess;

        ExcessRNs(DatacenterId dcId,
                  int requiredRF,
                  RepGroupId rgId,
                  int numExcess) {
            this.dcId = dcId;
            this.requiredRF = requiredRF;
            this.rgId = rgId;
            this.numExcess = numExcess;
        }

        @Override
        public ResourceId getResourceId() {
            return rgId;
        }

        @Override
        public String toString() {
            return rgId + " has " + numExcess +
                " more RNs than are needed for the required repFactor of " +
                requiredRF + " for " + dcId;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + ((dcId == null) ? 0 : dcId.hashCode());
            result = prime * result + numExcess;
            result = prime * result + requiredRF;
            result = prime * result + ((rgId == null) ? 0 : rgId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof ExcessRNs)) {
                return false;
            }
            ExcessRNs other = (ExcessRNs) obj;
            if (dcId == null) {
                if (other.dcId != null) {
                    return false;
                }
            } else if (!dcId.equals(other.dcId)) {
                return false;
            }
            if (numExcess != other.numExcess) {
                return false;
            }
            if (requiredRF != other.requiredRF) {
                return false;
            }
            if (rgId == null) {
                if (other.rgId != null) {
                    return false;
                }
            } else if (!rgId.equals(other.rgId)) {
                return false;
            }
            return true;
        }
    }

    /**
     * This SN has unused capacity slots.
     */
    public static class UnderCapacity extends Warning {
        private static final long serialVersionUID = 1L;
        private final StorageNodeId snId;
        private final int rnCount;
        private final int capacityVal;

        UnderCapacity(StorageNodeId snId, int rnCount, int capacityVal) {
            this.snId = snId;
            this.rnCount = rnCount;
            this.capacityVal = capacityVal;
        }

        @Override
        public ResourceId getResourceId() {
            return snId;
        }

        @Override
        public String toString() {
            return snId + " has " + rnCount +
                " RepNodes and is under its capacity limit of "+ capacityVal;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + capacityVal;
            result = prime * result + rnCount;
            result = prime * result + ((snId == null) ? 0 : snId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof UnderCapacity)) {
                return false;
            }
            UnderCapacity other = (UnderCapacity) obj;
            if (capacityVal != other.capacityVal) {
                return false;
            }
            if (rnCount != other.rnCount) {
                return false;
            }
            if (snId == null) {
                if (other.snId != null) {
                    return false;
                }
            } else if (!snId.equals(other.snId)) {
                return false;
            }
            return true;
        }
    }

    /**
     * This shard has too many or too few partitions.
     */
    public static class NonOptimalNumPartitions extends Warning {

        private static final long serialVersionUID = 1L;
        private final RepGroupId rgId;
        private final int actualCount;
        private final int minPartitions;
        private final int maxPartitions;


        NonOptimalNumPartitions(RepGroupId rgId, int actualCount,
                                int minPartitions, int maxPartitions) {
            this.rgId = rgId;
            this.actualCount = actualCount;
            this.minPartitions = minPartitions;
            this.maxPartitions = maxPartitions;
        }

        @Override
        public ResourceId getResourceId() {
            return rgId;
        }

        @Override
        public String toString() {
            return rgId + " should have " + minPartitions +
                    ((minPartitions != maxPartitions) ?
                            " to " + maxPartitions : "") +
                   " partitions if balanced, but has " + actualCount;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + actualCount;
            result = prime * result + maxPartitions;
            result = prime * result + minPartitions;
            result = prime * result + ((rgId == null) ? 0 : rgId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof NonOptimalNumPartitions)) {
                return false;
            }
            NonOptimalNumPartitions other = (NonOptimalNumPartitions) obj;
            if (actualCount != other.actualCount)
                return false;
            if (maxPartitions != other.maxPartitions)
                return false;
            if (minPartitions != other.minPartitions)
                return false;
            if (rgId == null) {
                if (other.rgId != null) {
                    return false;
                }
            } else if (!rgId.equals(other.rgId)) {
                return false;
            }
            return true;
        }
    }

   /**
     * This StorageNode has more than 1 RN housed in its root directory.
     * This is a configuration which could lead to I/O contention.
     */
    public static class MultipleRNsInRoot extends Warning {
        private static final long serialVersionUID = 1L;
        private final StorageNodeId snId;
        private final List<RepNodeId> residentRNs;
        private final String rootDir;

        MultipleRNsInRoot(StorageNodeId snId,
                          List<RepNodeId> residentRNs,
                          String rootDir) {

            this.snId = snId;
            this.residentRNs = residentRNs;
            this.rootDir = rootDir;
        }

        @Override
        public ResourceId getResourceId() {
            return snId;
        }

        /**
         * @return the list, formatted concisely as rn1, rn2, rn3
         */
        private String getRNList() {
            final StringBuilder sb = new StringBuilder();
            if (residentRNs != null) {
                for (RepNodeId rnId : residentRNs) {
                    if (sb.length() > 1) {
                        sb.append(", ");
                    }
                    sb.append(rnId);
                }
            }
            return sb.toString();
        }

        @Override
        public String toString() {
            return snId + " hosts " +
                plural((residentRNs == null ? 0 : residentRNs.size()), "RN")  +
                " (" + getRNList() + ") in " + rootDir +
                ". If this leads to insufficient I/O performance, consider " +
                "adding storage directories.";
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result
                    + ((residentRNs == null) ? 0 : residentRNs.hashCode());
            result = prime * result
                    + ((rootDir == null) ? 0 : rootDir.hashCode());
            result = prime * result + ((snId == null) ? 0 : snId.hashCode());
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof MultipleRNsInRoot)) {
                return false;
            }
            MultipleRNsInRoot other = (MultipleRNsInRoot) obj;
            if (residentRNs == null) {
                if (other.residentRNs != null) {
                    return false;
                }
            } else if (!residentRNs.equals(other.residentRNs)) {
                return false;
            }
            if (rootDir == null) {
                if (other.rootDir != null) {
                    return false;
                }
            } else if (!rootDir.equals(other.rootDir)) {
                return false;
            }
            if (snId == null) {
                if (other.snId != null) {
                    return false;
                }
            } else if (!snId.equals(other.snId)) {
                return false;
            }
            return true;
        }
    }

    /**
     * This zone has more Admins than repFactor requires.
     */
    public static class ExcessAdmins extends Warning {
        private static final long serialVersionUID = 1L;
        private final DatacenterId dcId;
        private final int requiredRF;
        private final int numExcess;

        ExcessAdmins(DatacenterId dcId,
                     int requiredRF,
                     int numExcess) {
            checkNull("dcId", dcId);
            if (requiredRF < 1) {
                throw new IllegalArgumentException(
                        "The value of requiredRF must be > 0");
            }
            if (numExcess < 1) {
                throw new IllegalArgumentException(
                        "The value of numExcess must be > 0");
            }
            this.dcId = dcId;
            this.requiredRF = requiredRF;
            this.numExcess = numExcess;
        }

        @Override
        public ResourceId getResourceId() {
            return dcId;
        }

        @Override
        public String toString() {
            return dcId + " has " + numExcess +
                " more Admins than are needed for the required repFactor of " +
                requiredRF;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + dcId.hashCode();
            result = prime * result + numExcess;
            result = prime * result + requiredRF;
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (!(obj instanceof ExcessAdmins)) {
                return false;
            }
            final ExcessAdmins other = (ExcessAdmins) obj;
            return dcId.equals(other.dcId) &&
                   numExcess == other.numExcess &&
                   requiredRF == other.requiredRF;
        }
    }

    /*
     * Other methods
     */

    private static String plural(int num, String noun) {
        if (num == 1) {
            return num + " " + noun;
        }

        return num + " " + noun + "s";
    }
}
