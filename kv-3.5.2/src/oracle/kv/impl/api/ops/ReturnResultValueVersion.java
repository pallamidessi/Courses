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

import oracle.kv.ReturnValueVersion.Choice;
import oracle.kv.Version;

/**
 * Holds ReturnValueVersion.Choice and ResultValueVersion during result
 * processing.  Initialized with the Choice specified in the request, and the
 * ResultValueVersion is filled in when the operation is complete.
 */
class ReturnResultValueVersion {

    private final Choice returnChoice;
    private ResultValueVersion valueVersion;

    ReturnResultValueVersion(Choice returnChoice) {
        this.returnChoice = returnChoice;
    }

    Choice getReturnChoice() {
        return returnChoice;
    }

    void setValueVersion(byte[] valueBytes, Version version) {
        this.valueVersion = new ResultValueVersion(valueBytes, version);
    }

    ResultValueVersion getValueVersion() {
        return valueVersion;
    }
}
