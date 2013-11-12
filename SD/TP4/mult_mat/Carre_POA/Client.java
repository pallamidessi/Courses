import java.io.*;
import org.omg.CORBA.*;

public class Client {
    public static void main( String args[] )  {
	int nombre;
	int res;
        if( args.length < 2 ) {
	    System.out.println( "Usage: java Client <ior> <nombre>" );
            System.exit( 1 );
        }

	// initialiser l'ORB.
	try {
			org.omg.CORBA.IntHolder intH = new org.omg.CORBA.IntHolder(); 
	    ORB orb = ORB.init( args, null );
	    System.out.println( "0) ORB initialise'");

	    String ior = args[0];
	    System.out.println( "1) IOR lue : " + ior );

	    org.omg.CORBA.Object obj = 
		orb.string_to_object(args[0]);
	    Icarre service = IcarreHelper.narrow(obj);
	    System.out.println("2) Reference obtenue a partir de l'IOR");

	    nombre = Integer.parseInt(args[1]);
	    System.out.println("3) Nombre lu sur la ligne de commande : "
			       + nombre);

	    service.carre(nombre,intH);

            System.out.println("4) Le serveur a trouve' un carre de : "
			       + intH.value);
	}
	catch( org.omg.CORBA.SystemException ex ) {
	      System.err.println( "Erreur !!" );
	      ex.printStackTrace();
	}
    }
}
