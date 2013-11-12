import org.omg.CORBA.*;
import org.omg.CosNaming.*;
import java.io.*;

// Exemple d'utilisation : java Client1 localhost 3000
public class Client {
    public static void main(String args[]) { 
	try {
	    if (args.length != 2) {
		System.out.println("Usage : java Client1 <machineServeurDeNoms> <port>");
		return;
	    }
	    String [] argv = {"-ORBInitialHost",args[0], 
			      "-ORBInitialPort",args[1]}; 
	    ORB orb = ORB.init( argv, null ); 
	    // ETAPE 1
	    org.omg.CORBA.Object obj = null;
	    obj = orb.resolve_initial_references("NameService");
	    if(obj == null) {
		System.out.println("Reference nil sur `NameService'");
		System.exit(1);
	    }
	    // ETAPE 2
	    NamingContext nc = NamingContextHelper.narrow(obj);
	    // ETAPE 3
	    NameComponent[] aName = new NameComponent[1];
	    aName[0] = new NameComponent();
	    aName[0].id = "Compte1";
	    aName[0].kind = "";
	    // ETAPE 4
	    obj=nc.resolve(aName);
	    // ETAPE 5
	    Comptes.Compte co = Comptes.CompteHelper.narrow(obj);

	    aName[0].id = "Compte2";
	    aName[0].kind = "";
	    // ETAPE 4
	    obj=nc.resolve(aName);
	    // ETAPE 5
	    Comptes.Compte co2 = Comptes.CompteHelper.narrow(obj);

      System.out.println("somme co1: "+co.afficheMontant()+",somme co2:"+co2.afficheMontant());
      co.virementCompteaCompte(50,co2);
      System.out.println("Virement ");
      System.out.println("somme co1: "+co.afficheMontant()+",somme co2:"+co2.afficheMontant());
	    obj=nc.resolve(aName);
      Comptes.Compte co3 = Comptes.CompteHelper.narrow(obj);
      System.out.println("somme co3: "+co3.afficheMontant());

	    if(co==null) {
		System.err.println("Erreur sur narrow() ");
		System.exit(0);
	    }
      System.out.println("Solde compte :"+co.afficheMontant());
      co.deposeBillets((float)100.);
      System.out.println("Solde compte :"+co.afficheMontant());
      co.retireBillets((float)150.);
      System.out.println("Solde compte :"+co.afficheMontant());
	} 
	catch( org.omg.CORBA.SystemException ex ) { ex.printStackTrace();}
	catch( org.omg.CORBA.UserException ex ) { ex.printStackTrace();}
    }
}
