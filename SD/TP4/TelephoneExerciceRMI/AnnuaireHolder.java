
/**
* AnnuaireHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from Annuaire.idl
* lundi 11 novembre 2013 12 h 43 CET
*/

public final class AnnuaireHolder implements org.omg.CORBA.portable.Streamable
{
  public Annuaire value = null;

  public AnnuaireHolder ()
  {
  }

  public AnnuaireHolder (Annuaire initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = AnnuaireHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    AnnuaireHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return AnnuaireHelper.type ();
  }

}
