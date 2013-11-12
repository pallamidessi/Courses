import org.omg.CORBA.*;
import java.awt.*; 
import java.applet.Applet;
import java.awt.event.*;
import java.io.*;

public class Lister {
    public static void main(String[] args) {
	if (args.length != 0) 
	    System.out.println("Usage : java Lister");
	else try {
	    
	    String ior;
	    Annuaire annuaire;
	    ORB orb = ORB.init(new String[0], null );
	    
	    FileReader file = new FileReader(iorfile.value);
	    BufferedReader in = new BufferedReader(file);
	    ior = in.readLine();
	    file.close();
	    
	    org.omg.CORBA.Object obj = 
		orb.string_to_object(ior);
	    annuaire = AnnuaireHelper.narrow(obj);
	    System.out.println(annuaire.listerNoms());
	}
	catch( Exception ex ) {
	    System.err.println( "Erreur !!" );
	    ex.printStackTrace();
	}
    }
}
