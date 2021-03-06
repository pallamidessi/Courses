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

package oracle.kv.table;

import java.util.Map;

/**
 * MapValue extends {@link FieldValue} to define a container object that holds
 * a map of FieldValue objects all of the same type.  The getters and setters
 * use the same semantics as Java Map.
 *
 * @since 3.0
 */
public interface MapValue extends FieldValue {

    /**
     * A constant used as a key for a map value when the value is used as part
     * of an IndexKey when there is an index on the value of a map's element or
     * a nested value within the element if the element is a record.
     */
    static final String ANONYMOUS = "[]";

    /**
     * Returns the MapDef that defines the content of this map.
     *
     * @return the MapDef
     */
    MapDef getDefinition();

    /**
     * Returns the size of the map.
     *
     * @return the size
     */
    int size();

    /**
     * Returns an unmodifiable view of the MapValue state.  The type of all
     * fields is the same and is defined by the {@link MapDef} returned by
     * {@link #getDefinition}.
     *
     * @return the map
     *
     * @since 3.0.6
     */
    Map<String, FieldValue> getFields();

    /**
     * Remove the named field if it exists.
     *
     * @param fieldName the name of the field to remove
     *
     * @return the FieldValue if it existed, otherwise null
     */
    FieldValue remove(String fieldName);

    /**
     * Returns the FieldValue with the specified name if it
     * appears in the map.
     *
     * @param fieldName the name of the desired field.
     *
     * @return the value for the field or null if the name does not exist in
     * the map.
     */
    FieldValue get(String fieldName);

    /**
     * Put a null value in the named field, silently overwriting
     * existing values.  Null values in maps are not normally allowed.  This
     * method is used for key-only map indexes and can be used by applications
     * to set a key without a value for lookup.  It is also returned by
     * key-only indexes.
     *
     * @param fieldName name of the desired field
     *
     * @return this
     *
     * @since 3.2
     */
    MapValue putNull(String fieldName);

    /**
     * Set the named field.  Any existing entry is silently overwritten.
     *
     * @param fieldName name of the desired field
     *
     * @param value the value to set
     *
     * @return this
     *
     * @throws IllegalArgumentException if the definition of the map type does
     * not match the input type
     */
    MapValue put(String fieldName, int value);

    /**
     * Set the named field.  Any existing entry is silently overwritten.
     *
     * @param fieldName name of the desired field
     *
     * @param value the value to set
     *
     * @return this
     *
     * @throws IllegalArgumentException if the definition of the map type does
     * not match the input type
     */
    MapValue put(String fieldName, long value);

    /**
     * Set the named field.  Any existing entry is silently overwritten.
     *
     * @param fieldName name of the desired field
     *
     * @param value the value to set
     *
     * @return this
     *
     * @throws IllegalArgumentException if the definition of the map type does
     * not match the input type
     */
    MapValue put(String fieldName, String value);

    /**
     * Set the named field.  Any existing entry is silently overwritten.
     *
     * @param fieldName name of the desired field
     *
     * @param value the value to set
     *
     * @return this
     *
     * @throws IllegalArgumentException if the definition of the map type does
     * not match the input type
     */
    MapValue put(String fieldName, double value);

    /**
     * Set the named field.  Any existing entry is silently overwritten.
     *
     * @param fieldName name of the desired field
     *
     * @param value the value to set
     *
     * @return this
     *
     * @throws IllegalArgumentException if the definition of the map type does
     * not match the input type
     */
    MapValue put(String fieldName, float value);

    /**
     * Set the named field.  Any existing entry is silently overwritten.
     *
     * @param fieldName name of the desired field
     *
     * @param value the value to set
     *
     * @return this
     *
     * @throws IllegalArgumentException if the definition of the map type does
     * not match the input type
     */
    MapValue put(String fieldName, boolean value);

    /**
     * Set the named field.  Any existing entry is silently overwritten.
     *
     * @param fieldName name of the desired field
     *
     * @param value the value to set
     *
     * @return this
     *
     * @throws IllegalArgumentException if the definition of the map type does
     * not match the input type
     */
    MapValue put(String fieldName, byte[] value);

    /**
     * Set the named field.  Any existing entry is silently overwritten.
     *
     * @param fieldName name of the desired field
     *
     * @param value the value to set
     *
     * @return this
     *
     * @throws IllegalArgumentException if the definition of the map type does
     * not match the input type
     */
    MapValue putFixed(String fieldName, byte[] value);

    /**
     * Set the named field.  Any existing entry is silently overwritten.
     *
     * @param fieldName name of the desired field
     *
     * @param value the value to set
     *
     * @return this
     *
     * @throws IllegalArgumentException if the definition of the map type does
     * not match the input type
     */
    MapValue putEnum(String fieldName, String value);

    /**
     * Set the named field.  Any existing entry is silently overwritten.
     *
     * @param fieldName name of the desired field
     *
     * @param value the value to set
     *
     * @return this
     *
     * @throws IllegalArgumentException if the definition of the map type does
     * not match the input type
     */
    MapValue put(String fieldName, FieldValue value);

    /**
     * Puts a Record into the map.  Existing values are silently overwritten.
     *
     * @param fieldName the field to use for the map key
     *
     * @return an uninitialized RecordValue that matches the type
     * definition for the map
     *
     * @throws IllegalArgumentException if the definition of the map type
     * is not a RecordDef
     */
    RecordValue putRecord(String fieldName);

    /**
     * Puts a Map into the map.  Existing values are silently overwritten.
     *
     * @param fieldName the field to use for the map key
     *
     * @return an uninitialized MapValue that matches the type
     * definition for the map
     *
     * @throws IllegalArgumentException if the definition of the map type
     * is not a MapDef
     */
    MapValue putMap(String fieldName);

    /**
     * Puts an Array into the map.  Existing values are silently overwritten.
     *
     * @param fieldName the field to use for the map key
     *
     * @return an uninitialized ArrayValue that matches the type
     * definition for the map
     *
     * @throws IllegalArgumentException if the definition of the map type
     * is not an ArrayDef
     */
    ArrayValue putArray(String fieldName);

    /**
     * Returns a deep copy of this object.
     *
     * @return a deep copy of this object
     */
    @Override
    public MapValue clone();

    /**
     * Returns a String representation of the value.  The value is returned
     * is a JSON string, and is the same as that returned by
     * {@link FieldValue#toJsonString}.
     *
     * @return a String representation of the value
     */
    @Override
    public String toString();
}
