import java.io.*;
import org.omg.CORBA.*;

public class Ajoute {
  public static void main(String [] args) {
	  if (args.length != 3) 
	    System.out.println("Usage : java Ajoute <IOR> <nom> <telephone>");
	  else {
	    try {
        // initialiser l'ORB.
        //org.omg.CORBA.IntHolder intH = new org.omg.CORBA.IntHolder(); 
        ORB orb = ORB.init( args, null );
        System.out.println( "0) ORB initialise'");
        
        //read the ior from the command-line
        String ior = args[0];
        System.out.println( "1) IOR lue : " + ior );
        
        org.omg.CORBA.Object obj = orb.string_to_object(args[0]);

        System.out.println("2) Reference obtenue a partir de l'IOR");
        Annuaire service = AnnuaireHelper.narrow(obj);
	      
        service.ajouteNom(args[1],args[2]);
	      System.out.println("Ajoute : "+args[1]+" "+args[2]);
	    }
      catch(Exception e){}
	  }
  }
}
