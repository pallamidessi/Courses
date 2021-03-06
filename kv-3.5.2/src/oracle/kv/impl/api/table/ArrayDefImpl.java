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

package oracle.kv.impl.api.table;

import static oracle.kv.impl.api.table.TableJsonUtils.COLLECTION;

import java.util.ListIterator;

import oracle.kv.impl.util.JsonUtils;
import oracle.kv.table.ArrayDef;
import oracle.kv.table.FieldDef;

import org.codehaus.jackson.JsonNode;
import org.codehaus.jackson.node.ObjectNode;

import com.sleepycat.persist.model.Persistent;

/**
 * ArrayDefImpl implements the ArrayDef interface.
 */
@Persistent(version=1)
class ArrayDefImpl extends FieldDefImpl implements ArrayDef {

    private static final long serialVersionUID = 1L;
    private final FieldDefImpl element;

    ArrayDefImpl(FieldDefImpl element,
                 String description) {
        super(FieldDef.Type.ARRAY, description);
        if (element == null) {
            throw new IllegalArgumentException
                ("Array has no field and cannot be built");
        }
        this.element = element;
    }

    /**
     * This constructor is only used by test code.
     */
    ArrayDefImpl(FieldDefImpl element) {
        this(element, null);
    }

    private ArrayDefImpl(ArrayDefImpl impl) {
        super(impl);
        element = impl.element.clone();
    }

    @SuppressWarnings("unused")
    private ArrayDefImpl() {
        element = null;
    }

    @Override
    public FieldDef getElement() {
        return element;
    }

    @Override
    public boolean isArray() {
        return true;
    }

    @Override
    public ArrayDef asArray() {
        return this;
    }

    /**
     * Arrays are allowed to be indexed if the array contain a type that is
     * allowed in an index.
     */
    @Override
    public boolean isValidIndexField() {
        return (element.isValidIndexField() &&
                !element.isArray() && !element.isMap());
    }

    @Override
    public boolean equals(Object other) {
        if (other instanceof ArrayDefImpl) {
            return element.equals(((ArrayDefImpl)other).getElement());
        }
        return false;
    }

    @Override
    public int hashCode() {
        return element.hashCode();
    }

    @Override
    public ArrayDefImpl clone() {
        return new ArrayDefImpl(this);
    }

    @Override
    public ArrayValueImpl createArray() {
        return new ArrayValueImpl(this);
    }

    @Override
    void toJson(ObjectNode node) {
        super.toJson(node);
        ObjectNode collNode = node.putObject(COLLECTION);
        if (element != null) {
            element.toJson(collNode);
        }
    }

    /**
     * {
     *  "type": {
     *    "type" : "array",
     *    "items" : "simpleType"  or for a complex type
     *    "items" : {
     *        "type" : ...
     *        ...
     *     }
     *  }
     * }
     */
    @Override
    public JsonNode mapTypeToAvro(ObjectNode node) {
        if (node == null) {
            node = JsonUtils.createObjectNode();
        }
        node.put("type", "array");
        node.put("items", element.mapTypeToAvroJsonNode());
        return node;
    }

    @Override
    FieldValueImpl createValue(JsonNode node) {
        if (node == null || node.isNull()) {
            return NullValueImpl.getInstance();
        }
        if (!node.isArray()) {
            throw new IllegalArgumentException
                ("Default value for type ARRAY is not an array");
        }
        if (node.size() != 0) {
            throw new IllegalArgumentException
                ("Default value for array must be null or an empty array");
        }
        return createArray();
    }

   /**
     * Find the FieldDef defined by the list of field names.
     *
     * Arrays are odd in that they have no field names, so when this function is
     * called its own name has already been consumed by the caller so the name
     * is passed directly to the element.
     *
     * If, somehow, this is called when the element is a simple type it's
     * findField method will return null, which is handled by the caller.
     *
     * Examples:
     * arrayField.a -- address the "a" field of the array's element, which must
     *    be a map or record
     * arrayField.a.b address the "b" field of the field contained in the "a"
     *    field of the array's element.
     */
    @Override
    FieldDefImpl findField(ListIterator<String> fieldPath) {

        /*
         * Peek at the current component.  If it is [], consume it,
         * and keep going. This allows operations to target the element
         * itself vs a field contained in the element.
         */
        String component = fieldPath.next();
        if (MapDefImpl.isMapValueTag(component)) {
            if (!fieldPath.hasNext()) {
                return element;
            }
        } else {
            /* restore the state for the element to use */
            fieldPath.previous();
        }

        /*
         * Do not consume the current name, and pass everything to the element.
         */
        return element.findField(fieldPath);
    }

    /**
     * If called for an array the fieldName applies to a field in the array's
     * element, so pass it on.
     */
    @Override
    FieldDefImpl findField(String fieldName) {
        return element.findField(fieldName);
    }
}
