import org.omg.CORBA.*;

public class IcarreImpl extends IcarrePOA {
  public void carre (int source, org.omg.CORBA.IntHolder result) {
    System.out.println( "carre : requete recue pour le nombre : " 
        + source );
    result.value=source*source;
  }
}
