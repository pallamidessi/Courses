import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import java.util.*;
import java.io.*;

public class AnnuaireImpl 
    extends AnnuairePOA {

    Hashtable numeros = new Hashtable();

    public AnnuaireImpl() {
	numeros.put("tintin", "06 76 70 80 09");
	numeros.put("milou", "06 50 40 36 76");
	numeros.put("tournesol", "06 07 33 72 06");
    }
    public String listerNoms () {
	String res = " " ;
	Iterator it = numeros.keySet().iterator();
	while (it.hasNext()) {
	    String nom = (String) it.next();
	    res += nom + " : " + numeros.get(nom) + " \n " ;
	}
	return res;
    }
    public String chercheNom(String nom) {    
	String resultat = (String) numeros.get(nom); 
	if (resultat == null) resultat = "Nom inconnu : " + nom;
	return resultat;
    }
    public void enleveNom(String nom) {
	numeros.remove(nom); 
    }
    public void ajouteNom(String nom, String num) { 
	String resultat = 
	    (String) numeros.put(nom,num); 
    }
    public static void main(String[] args) {
	try {
            //init ORB
	    ORB orb = ORB.init( args, null );
	    
            AnnuaireImpl myobj = new AnnuaireImpl ();

	    //initialiser le POA
	    POA poa = 
                POAHelper.narrow( orb.resolve_initial_references( "RootPOA" ));

	    poa.the_POAManager().activate();

            org.omg.CORBA.Object poaobj = 
                poa.servant_to_reference( myobj );

	    String ior = orb.object_to_string( poaobj );
	    
	    FileOutputStream file = new FileOutputStream(iorfile.value);
	    PrintWriter out = new PrintWriter(file);
	    out.println(ior);out.flush();
	    file.close();
	    
      	    orb.run();
	}
	catch( Exception ex ) {
	    System.err.println( "Erreur !!" );
	    ex.printStackTrace();
	}
    }
}
