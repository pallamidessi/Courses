import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import java.io.*;

public class Supprime {
  public static void main(String [] args) {
    if (args.length != 1) 
      System.out.println("Usage : java Supprime <nom>");
    else {
      try {
        String ior;
        // initialiser l'ORB.
        //org.omg.CORBA.IntHolder intH = new org.omg.CORBA.IntHolder(); 
        ORB orb = ORB.init( args, null );
        System.out.println( "0) ORB initialise'");

        //read the ior from a file
        FileReader file = new FileReader(iorfile.value) ;
        BufferedReader in = new BufferedReader(file) ;
        ior = in.readLine() ;
        file.close() ;
        System.out.println( "1) IOR lue : " + ior );

        System.out.println("2) Reference obtenue a partir de l'IOR");
        org.omg.CORBA.Object obj = orb.string_to_object(ior) ;
        Annuaire annuaire = AnnuaireHelper.narrow(obj);


        System.out.println("Supprime : "+args[0]);
        annuaire.enleveNom(args[0]);
      }
      catch(Exception e){}
    }
  }
}
