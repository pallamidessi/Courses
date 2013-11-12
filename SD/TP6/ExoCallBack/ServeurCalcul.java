import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import java.util.*;
import java.io.*;
import org.omg.CosNaming.*;
import Callback.*;

class CalculantImpl extends CalculantPOA {
  private ORB orb;
  private Vector<Callback.Afficheur> registredClient;

  private double accumulateur = 1.0;
  public void faitLeCalcul (double val, Callback.Afficheur peer) {
    accumulateur+=val;
    peer.afficheRes("la valeur courante est "+accumulateur);
    
    while(true){
      peer.afficheRes("la valeur courante est "+accumulateur);
    }

   // peer.arreteORB();

  }

  public void inscription(Callback.Afficheur peer){
    registredClient.add(peer);
    //verifié si ce n'est pas deja dedans
  }
  
}

public class ServeurCalcul {
  public static void main(String args[]) {
    try{
      if (args.length<2) {
        System.out.println("Usage ServeurCalcul: <machine serveur de nom> <port>");
      }
	    String [] argv = {"-ORBInitialHost",args[0], 
			      "-ORBInitialPort",args[1]}; 

      ORB orb = ORB.init(argv, null);
      
      // init POA
      POA rootpoa =POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootpoa.the_POAManager().activate();
      
      // creer l'objet qui sera appele' depuis le client
      CalculantImpl c1 = new CalculantImpl();
      org.omg.CORBA.Object ref = rootpoa.servant_to_reference(c1);
      Calculant href = CalculantHelper.narrow(ref);
      
      // inscription de l'objet au service de noms
      org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
      NameComponent path[] = ncRef.to_name( "Cerveau" );
      ncRef.rebind(path, href);
      System.out.println("Serveur pret ...");
      orb.run();
    } 
    catch (Exception e) {
      System.err.println("ERROR: " + e);
      e.printStackTrace(System.out);
    }
    System.out.println("Fin Serveur  ..."); 
  }
}











