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

import java.io.Serializable;
import java.util.List;

import oracle.kv.impl.admin.VerifyConfiguration.Problem;

/**
 * Return progress information and results from a verification run.
 */
public class VerifyResults implements Serializable {
    private static final long serialVersionUID = 1L;
    private final List<Problem> violations;
    private final List<Problem> warnings;
    private final String progressReport;

    public VerifyResults(String progressReport,
                         List<Problem> violations,
                         List<Problem> warnings) {
        this.progressReport = progressReport;
        this.violations = violations;
        this.warnings = warnings;
    }

    VerifyResults(List<Problem> violations,
                  List<Problem> warnings) {
        this.progressReport = null;
        this.violations = violations;
        this.warnings = warnings;
    }

    public int numWarnings() {
        return warnings.size();
    }

    public List<Problem> getViolations() {
        return violations;
    }

    public int numViolations() {
        return violations.size();
    }

    public List<Problem> getWarnings() {
        return warnings;
    }

    public boolean okay() {
        return (violations.size() == 0) && (warnings.size() == 0);
    }

    public String display() {
        return progressReport;
    }
}
