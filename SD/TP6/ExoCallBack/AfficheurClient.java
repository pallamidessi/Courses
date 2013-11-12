import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import java.util.*;
import java.io.*;
import org.omg.CosNaming.*;
import Callback.*;

class AfficheurImpl extends AfficheurPOA {
  private ORB orb;
  public AfficheurImpl(ORB orb) {
    this.orb = orb;
  }
  public void afficheRes (String res) {
    System.out.println("Le resultat "+res);
  }
  public void arreteORB () {
    orb.shutdown(false);   
  }
}

class ThreadRun extends Thread {
  private ORB orb;
  public ThreadRun(ORB orb) {
    this.orb = orb;
  }
  public void run() {
    try{
      orb.run();
    } catch (Exception e) {
      System.out.println("ERROR : " + e) ;
      e.printStackTrace(System.out);
      System.exit(1);
    }
  }
}

public class AfficheurClient {
  public static void main(String args[]) {
    try{
      if (args.length<2) {
        System.out.println("Usage ServeurCalcul: <machine serveur de nom> <port>");
      }
	    String [] argv = {"-ORBInitialHost",args[0], 
			      "-ORBInitialPort",args[1]}; 
      ORB orb = ORB.init(argv, null);
      
      // Init POA
      POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootpoa.the_POAManager().activate();
      
      // creer l'objet qui sera appele' depuis le serveur
      AfficheurImpl a1 = new AfficheurImpl(orb);
      org.omg.CORBA.Object ref = rootpoa.servant_to_reference(a1);
      Afficheur cbref = AfficheurHelper.narrow(ref); 
      
      // obtenir reference sur l'objet distant
      org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
      Calculant c1=CalculantHelper.narrow(ncRef.resolve_str("Cerveau"));
      
      // lancer l'ORB dans un thread
      ThreadRun thread = new ThreadRun(orb);
      thread.start();
      
      // declenche la methode sur l'objet distant
      double val;
      if (args.length >=1) val=Double.parseDouble(args[2]);
      else val=1.0; 
        c1.faitLeCalcul(val, cbref);
    } catch (Exception e) {
        System.out.println("ERROR : " + e) ;
        e.printStackTrace(System.out);	
    }
  }
}
