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

package oracle.kv.impl.admin.client;

import java.rmi.RemoteException;
import java.util.HashSet;
import java.util.Set;

import oracle.kv.impl.admin.CommandServiceAPI;
import oracle.kv.impl.topo.AdminId;
import oracle.kv.impl.topo.DatacenterId;
import oracle.kv.impl.util.ConfigurableService.ServiceStatus;
import oracle.kv.impl.util.ServiceUtils;
import oracle.kv.util.shell.Shell;
import oracle.kv.util.shell.ShellCommand;
import oracle.kv.util.shell.ShellException;
import oracle.kv.util.shell.ShellUsageException;

class RepairAdminQuorumCommand extends ShellCommand {

    static final String COMMAND_NAME = "repair-admin-quorum";

    static final String COMMAND_SYNTAX =
        COMMAND_NAME + " {-zn <id>|-znname <name>|-admin <id>}...";

    static final String COMMAND_DESC =
        "Repairs admin quorum by reducing membership of the admin group to" +
        " the" + eolt + "admins in the specified zones or the specific" +
        " admins listed. This" + eolt + "command should be used when" +
        " attempting to recover from a failure that" + eolt + "has resulted" +
        " in a loss of admin quorum.";

    RepairAdminQuorumCommand() {

        /* Allow abbeviation as "repair-a". */
        super(COMMAND_NAME, 8);
    }

    @Override
    public String execute(String[] args, Shell shell)
        throws ShellException {

        Shell.checkHelp(args, this);
        final CommandShell cmd = (CommandShell) shell;

        final Set<DatacenterId> zoneIds = new HashSet<DatacenterId>();
        final Set<String> zoneNames = new HashSet<String>();
        final Set<AdminId> adminIds = new HashSet<AdminId>();
        boolean specifiedAdmins = false;
        for (int i = 1; i < args.length; i++) {
            String arg = args[i];
            if ("-zn".equals(arg)) {
                zoneIds.add(parseDatacenterId(Shell.nextArg(args, i++, this)));
                specifiedAdmins = true;
            } else if ("-znname".equals(arg)) {
                zoneNames.add(Shell.nextArg(args, i++, this));
                specifiedAdmins = true;
            } else if ("-admin".equals(arg)) {
                adminIds.add(parseAdminid(Shell.nextArg(args, i++, this)));
                specifiedAdmins = true;
            } else {
                shell.unknownArgument(arg, this);
            }
        }
        if (!specifiedAdmins) {
            throw new ShellUsageException(
                "Need to specify -zn, -znname, or -admin flags",
                this);
        }
        try {
            Set<AdminId> requestedAdmins =
                repairAdminQuorum(cmd, zoneIds, zoneNames, adminIds);
            return "Repaired admin quorum using admins: " + requestedAdmins;
        } catch (Exception e) {
            throw new ShellException("Problem repairing admin quorum: " +
                                     e.getMessage(),
                                     e);
        }
    }

    /** Make repair call through a method to simplify testing. */
    Set<AdminId> repairAdminQuorum(CommandShell cmd,
                                   Set<DatacenterId> zoneIds,
                                   Set<String> zoneNames,
                                   Set<AdminId> adminIds)
        throws ShellException {

        try {
            CommandServiceAPI cs = cmd.getAdmin();
            for (final String zoneName : zoneNames) {
                zoneIds.add(CommandUtils.getDatacenterId(zoneName, cs, this));
            }
            Set<AdminId> result = cs.repairAdminQuorum(zoneIds, adminIds);
            if (result != null) {
                return result;
            }

            /* Retry the command after the current admin is restarted */
            try {
                ServiceUtils.waitForAdmin(cmd.getAdminHostname(),
                                          cmd.getAdminPort(),
                                          null, /* loginManager */
                                          90, /* timeoutSec */
                                          ServiceStatus.RUNNING);
            } catch (Exception e) {
                /* Let the repairAdminQuorum call signal the problem */
            }
            cs = cmd.getAdmin();
            result = cs.repairAdminQuorum(zoneIds, adminIds);
            if (result != null) {
                return result;
            }

            /*
             * Only do one retry, since multiple restarts of the current admin,
             * while possible, are not expected
             */
            throw new ShellException(
                "Problem repairing admin quorum: " +
                "The command needs to be retried manually because" +
                " there were multiple restarts of the current admin");
        } catch (RemoteException e) {
            cmd.noAdmin(e);
            throw new AssertionError("Not reached");
        }
    }

    @Override
    protected String getCommandSyntax() {
        return COMMAND_SYNTAX;
    }

    @Override
    protected String getCommandDescription() {
        return COMMAND_DESC;
    }
}
