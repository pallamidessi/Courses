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

package oracle.kv.impl.admin;

import oracle.kv.impl.fault.CommandFaultException;
import oracle.kv.util.ErrorMessage;

/**
 * Exception to indicate that an administrative command issued by the user has
 * an illegal argument, or is not permissible at the current time or in the
 * current state of the store.
 *
 * The problem should be reported back to the user, and the user should reissue
 * the command.
 */
public class IllegalCommandException extends CommandFaultException {

    private static final long serialVersionUID = 1L;

    private static final ErrorMessage DEFAULT_ERR_MSG =
        ErrorMessage.NOSQL_5100;
    private static final String[] EMPTY_CLEANUP_JOBS = new String[] {};

    public IllegalCommandException(String message) {
        this(message, DEFAULT_ERR_MSG, EMPTY_CLEANUP_JOBS);
    }

    public IllegalCommandException(String message, Throwable t) {
        this(message, t, DEFAULT_ERR_MSG, EMPTY_CLEANUP_JOBS);
    }

    public IllegalCommandException(String message,
                                   ErrorMessage errorMsg,
                                   String[] cleanupJobs) {
        super(message, errorMsg, cleanupJobs);
    }

    public IllegalCommandException(String message,
                                   Throwable t,
                                   ErrorMessage errorMsg,
                                   String[] cleanupJobs) {
        super(message, t, errorMsg, cleanupJobs);
    }
}
