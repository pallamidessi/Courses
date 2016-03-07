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

package oracle.kv.util.shell;

import java.rmi.RemoteException;
import java.util.Set;

import oracle.kv.impl.admin.CommandServiceAPI;
import oracle.kv.impl.admin.client.CommandUtils;
import oracle.kv.impl.topo.AdminId;
import oracle.kv.impl.topo.DatacenterId;
import oracle.kv.impl.topo.DatacenterType;
import oracle.kv.impl.topo.RepNodeId;
import oracle.kv.impl.topo.StorageNodeId;
import oracle.kv.util.shell.Shell.VariablesMap;

/**
 * Base abstract class for all shell commands.
 */
public abstract class ShellCommand implements Cloneable{

    /* Convenience */
    protected final static String eol = Shell.eol;
    protected final static String eolt = Shell.eolt;

    /* The command name */
    protected final String name;

    /*
     * The number of characters which must match when matching the
     * command name.
     */
    protected final int prefixMatchLength;

    protected VariablesMap cmdVariables = null;

    protected int exitCode = Shell.EXIT_OK;

    private String prompt = null;

    /*
     * Indicates whether this command or its subcommands would like to parse
     * the "-json" flag in own way.
     */
    protected boolean overrideJsonFlag = false;

    /**
     * Constructor.
     *
     * @param name the command name
     * @param prefixMatchLength number of characters to match the name
     */
    protected ShellCommand(String name, int prefixMatchLength) {
        assert name.length() >= prefixMatchLength;

        this.name = name;
        this.prefixMatchLength = prefixMatchLength;
        this.cmdVariables = new VariablesMap();
    }

    /**
     * Returns true if the command is hidden. The default implementation
     * returns false.
     *
     * @return true if the command is hidden
     */
    protected boolean isHidden() {
        return false;
    }

    /**
     * Returns true if the command is deprecated. The default implementation
     * returns false.
     *
     * @return true if the command is deprecated
     */
    protected boolean isDeprecated() {
        return false;
    }

    /**
     * Gets the command name. The default implementation returns the name
     * parameter specified in the constructor.
     *
     * @return the command name
     */
    protected String getCommandName() {
        return name;
    }

    /**
     * Gets the string describing the command's syntax. The default
     * implementation returns the string returned by getCommandName().
     *
     * @return the command syntax
     */
    protected String getCommandSyntax() {
        return getCommandName();
    }

    /**
     * Gets the string description for this command.
     *
     * @return the command description
     */
    protected abstract String getCommandDescription();

    /**
     * Returns true if the specified command name matches this command.
     *
     * @param commandName the command name
     * @return true if the specified command name matches this command
     */
    protected boolean matches(String commandName) {
        return Shell.matches(commandName, name, prefixMatchLength);
    }

    /**
     * Execute this command.
     *
     * @param args
     * @param shell
     * @return the result of the command
     * @throws ShellException
     */
    public abstract String execute(String[] args, Shell shell)
        throws ShellException;

    /**
     * Gets the help string based on the specified arguments. The default
     * implementation returns string returned by getVerboseHelp(), ignoring
     * the input arguments.
     *
     * @param args
     * @param shell
     * @return the help string
     */
    protected String getHelp(String[] args, Shell shell) {
        return getVerboseHelp();
    }

    /**
     * Gets an expanded help string. The default implementation returns the
     * string returned by getBriefHelp() followed by EOL, TAB, then the string
     * returned by getCommandDescription().
     *
     * @return the help string
     */
    protected String getVerboseHelp() {
        return getBriefHelp() + eolt + getCommandDescription();
    }

    /**
     * Gets an abbreviated help string. The default implementation returns
     * the string "Usage: " followed by the string returned by
     * getCommandSyntax().
     *
     * @return an abbreviated help string
     */
    protected String getBriefHelp() {
        return "Usage: " + getCommandSyntax();
    }

    /**
     * Set a customized prompt string.
     */
    public void setPrompt(String myPrompt) {
        prompt = myPrompt;
    }

    /**
     * Gets a customized prompt string.
     *
     * @return the prompt string
     */
    public String getPrompt() {
        return prompt;
    }

    @Override
    public ShellCommand clone() {
        try {
            ShellCommand cmd = (ShellCommand) super.clone();
            cmd.cmdVariables = this.cmdVariables.clone();
            return cmd;
        } catch (CloneNotSupportedException neverHappens) {
            return null;
        }
    }

    /**
     * Store a variable.
     */
    public void addVariable(String varName, Object value) {
        cmdVariables.add(varName, value);
    }

    /**
     * Remove a variable.
     */
    public void removeVariable(String varName) {
        cmdVariables.remove(varName);
    }

    /**
     * Remove all variables.
     */
    public void clearVariables() {
        cmdVariables.reset();
    }

    /**
     * Get the value of a variable.
     */
    public Object getVariable(String varName) {
        return cmdVariables.get(varName);
    }

    public void invalidArgument(String arg)
        throws ShellException {

        String msg = "Invalid argument: " + arg + eolt + getBriefHelp();
        throw new ShellArgumentException(msg);
    }

    // TODO: Maybe provide an overloading of these methods that allows the
    // caller to identify the argument if there is a failure?

    /**
     * Parses the string argument as a signed decimal integer. Throws a
     * ShellArgumentException with the brief command help if the argument
     * does not contain a parseable int.
     */
    public int parseInt(String arg)
        throws ShellException {
        try {
            return Integer.parseInt(arg);
        } catch (NumberFormatException nfe) {
            invalidArgument(arg);
        }
        return 0; /* Not reached */
    }

    /**
     * Parses the string argument as a unsigned (positive) decimal integer.
     * Throws a ShellArgumentException with the brief command help if the
     * argument does not contain a parseable int or the value is < 0.
     */
    public int parseUnsignedInt(String arg)
        throws ShellException {

        final int value = parseInt(arg);
        if (value < 0) {
            invalidArgument(arg);
        }
        return value;
    }

    /**
     * Parses the string argument as a signed decimal long. Throws a
     * ShellArgumentException with the brief command help if the argument
     * does not contain a parseable long.
     */
    public long parseLong(String arg)
        throws ShellException {
        try {
            return Long.parseLong(arg);
        } catch (NumberFormatException nfe) {
            invalidArgument(arg);
        }
        return 0; /* Not reached */
    }

    /**
     * Parses the string argument as a unsigned (positive) decimal long.
     * Throws a ShellArgumentException with the brief command help if the
     * argument does not contain a parseable long or the value is < 0.
     */
    public long parseUnsignedLong(String arg)
        throws ShellException {

        final long value = parseLong(arg);
        if (value < 0) {
            invalidArgument(arg);
        }
        return value;
    }

    public int getExitCode() {
        return exitCode;
    }

    public boolean overrideJsonFlag() {
        return overrideJsonFlag;
    }

    protected void validateRepNodes(CommandServiceAPI cs, Set<RepNodeId> rnids)
        throws RemoteException, ShellException {

        for (RepNodeId rnid : rnids) {
            CommandUtils.ensureRepNodeExists(rnid, cs, this);
        }
    }

    protected StorageNodeId parseSnid(String idString)
        throws ShellException {

        try {
            return StorageNodeId.parse(idString);
        } catch (IllegalArgumentException ignored) {
            throw new ShellUsageException(
                "Invalid storage node ID: " + idString, this);
        }
    }

    protected DatacenterId parseDatacenterId(String idString)
        throws ShellException {

        try {
            return DatacenterId.parse(idString);
        } catch (IllegalArgumentException ignored) {
            throw new ShellUsageException(
                "Invalid zone ID: " + idString, this);
        }
    }

    protected RepNodeId parseRnid(String idString)
        throws ShellException {

        try {
            return RepNodeId.parse(idString);
        } catch (IllegalArgumentException ignored) {
            throw new ShellUsageException(
                "Invalid RepNode ID: " + idString, this);
        }
    }

    protected AdminId parseAdminid(String idString)
        throws ShellException {

        try {
            return AdminId.parse(idString);
        } catch (IllegalArgumentException ignored) {
            throw new ShellUsageException(
                "Invalid Admin ID: " + idString, this);
        }
    }

    protected DatacenterType parseDatacenterType(final String string)
        throws ShellException {

        try {
            return DatacenterType.valueOf(string.toUpperCase());
        } catch (IllegalArgumentException ignored) {
            throw new ShellUsageException("Invalid zone type: " + string,
                                          this);
        }
    }
}
