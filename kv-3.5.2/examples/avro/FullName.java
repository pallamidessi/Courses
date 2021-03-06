/**
 * Autogenerated by Avro
 * 
 * DO NOT EDIT DIRECTLY
 */
package avro;  
@SuppressWarnings("all")
public class FullName extends org.apache.avro.specific.SpecificRecordBase implements org.apache.avro.specific.SpecificRecord {
  public static final org.apache.avro.Schema SCHEMA$ = new org.apache.avro.Schema.Parser().parse("{\"type\":\"record\",\"name\":\"FullName\",\"namespace\":\"avro\",\"fields\":[{\"name\":\"first\",\"type\":{\"type\":\"string\",\"avro.java.string\":\"String\"},\"default\":\"\"},{\"name\":\"last\",\"type\":{\"type\":\"string\",\"avro.java.string\":\"String\"},\"default\":\"\"}]}");
  @Deprecated public java.lang.String first;
  @Deprecated public java.lang.String last;
  public org.apache.avro.Schema getSchema() { return SCHEMA$; }
  // Used by DatumWriter.  Applications should not call. 
  public java.lang.Object get(int field$) {
    switch (field$) {
    case 0: return first;
    case 1: return last;
    default: throw new org.apache.avro.AvroRuntimeException("Bad index");
    }
  }
  // Used by DatumReader.  Applications should not call. 
  @SuppressWarnings(value="unchecked")
  public void put(int field$, java.lang.Object value$) {
    switch (field$) {
    case 0: first = (java.lang.String)value$; break;
    case 1: last = (java.lang.String)value$; break;
    default: throw new org.apache.avro.AvroRuntimeException("Bad index");
    }
  }

  /**
   * Gets the value of the 'first' field.
   */
  public java.lang.String getFirst() {
    return first;
  }

  /**
   * Sets the value of the 'first' field.
   * @param value the value to set.
   */
  public void setFirst(java.lang.String value) {
    this.first = value;
  }

  /**
   * Gets the value of the 'last' field.
   */
  public java.lang.String getLast() {
    return last;
  }

  /**
   * Sets the value of the 'last' field.
   * @param value the value to set.
   */
  public void setLast(java.lang.String value) {
    this.last = value;
  }

  /** Creates a new FullName RecordBuilder */
  public static avro.FullName.Builder newBuilder() {
    return new avro.FullName.Builder();
  }
  
  /** Creates a new FullName RecordBuilder by copying an existing Builder */
  public static avro.FullName.Builder newBuilder(avro.FullName.Builder other) {
    return new avro.FullName.Builder(other);
  }
  
  /** Creates a new FullName RecordBuilder by copying an existing FullName instance */
  public static avro.FullName.Builder newBuilder(avro.FullName other) {
    return new avro.FullName.Builder(other);
  }
  
  /**
   * RecordBuilder for FullName instances.
   */
  public static class Builder extends org.apache.avro.specific.SpecificRecordBuilderBase<FullName>
    implements org.apache.avro.data.RecordBuilder<FullName> {

    private java.lang.String first;
    private java.lang.String last;

    /** Creates a new Builder */
    private Builder() {
      super(avro.FullName.SCHEMA$);
    }
    
    /** Creates a Builder by copying an existing Builder */
    private Builder(avro.FullName.Builder other) {
      super(other);
    }
    
    /** Creates a Builder by copying an existing FullName instance */
    private Builder(avro.FullName other) {
            super(avro.FullName.SCHEMA$);
      if (isValidValue(fields()[0], other.first)) {
        this.first = (java.lang.String) data().deepCopy(fields()[0].schema(), other.first);
        fieldSetFlags()[0] = true;
      }
      if (isValidValue(fields()[1], other.last)) {
        this.last = (java.lang.String) data().deepCopy(fields()[1].schema(), other.last);
        fieldSetFlags()[1] = true;
      }
    }

    /** Gets the value of the 'first' field */
    public java.lang.String getFirst() {
      return first;
    }
    
    /** Sets the value of the 'first' field */
    public avro.FullName.Builder setFirst(java.lang.String value) {
      validate(fields()[0], value);
      this.first = value;
      fieldSetFlags()[0] = true;
      return this; 
    }
    
    /** Checks whether the 'first' field has been set */
    public boolean hasFirst() {
      return fieldSetFlags()[0];
    }
    
    /** Clears the value of the 'first' field */
    public avro.FullName.Builder clearFirst() {
      first = null;
      fieldSetFlags()[0] = false;
      return this;
    }

    /** Gets the value of the 'last' field */
    public java.lang.String getLast() {
      return last;
    }
    
    /** Sets the value of the 'last' field */
    public avro.FullName.Builder setLast(java.lang.String value) {
      validate(fields()[1], value);
      this.last = value;
      fieldSetFlags()[1] = true;
      return this; 
    }
    
    /** Checks whether the 'last' field has been set */
    public boolean hasLast() {
      return fieldSetFlags()[1];
    }
    
    /** Clears the value of the 'last' field */
    public avro.FullName.Builder clearLast() {
      last = null;
      fieldSetFlags()[1] = false;
      return this;
    }

    @Override
    public FullName build() {
      try {
        FullName record = new FullName();
        record.first = fieldSetFlags()[0] ? this.first : (java.lang.String) defaultValue(fields()[0]);
        record.last = fieldSetFlags()[1] ? this.last : (java.lang.String) defaultValue(fields()[1]);
        return record;
      } catch (Exception e) {
        throw new org.apache.avro.AvroRuntimeException(e);
      }
    }
  }
}
