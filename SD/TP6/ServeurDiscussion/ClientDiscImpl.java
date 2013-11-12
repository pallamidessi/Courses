import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import java.util.*;
import java.io.*;
import java.io.IOException;
import org.omg.CosNaming.*;
import Discussion.ClientDisc;
import Discussion.ServeurDisc;
import Discussion.ServeurDiscHelper;
import Discussion.ClientDiscPOA;
import Discussion.ClientDiscHelper;

public class ClientDiscImpl extends ClientDiscPOA {
    String my_name = "chatter";
    BufferedReader br = null;
    ClientDisc chatter;
    ServeurDisc serveur;
    ThreadRun thread;

    public void receiveEnter (String name) {
	display("# "+name+" entered");
    }

    public void receiveExit (String name) {
	display("# "+name+" left");
    }

    public void receiveChat (String name, String message) {
	display(name+": "+message);
    }

    private void display(String s) {
	System.out.println(s);
    }

    private String getEntry() throws IOException {
	String s = null;
	s = br.readLine();
	return (s);
    }

    private void loop() {
	try {
	    boolean got_name = false;
	    InputStreamReader isr = null;
	    String message = " ";

	    isr = new InputStreamReader (System.in);
	    br = new BufferedReader (isr);
	    while (message.equals("quit") == false) {
		message=getEntry();
		if (message == null) message = " ";
		else {
		    if (got_name) {
			// A completer
		    } else {
			got_name = true;
			my_name = message;
			// A completer
		    }
		}
	    }
	} catch (IOException e) {
	    System.out.println("ERROR : " + e) ;
	    e.printStackTrace();
	} finally {
	    if (serveur != null) serveur.logout(my_name);
	}
    }

    public static void main(String args[]) {
	ClientDiscImpl client = null;

	if (args.length != 1) {
	    System.out.println("Usage : java ServeurDiscImpl"+
			       " <machineServeurDeNoms>");
	    return;
	}
	try{
	    String [] argv = {"-ORBInitialHost",args[0], 
			      "-ORBInitialPort","1050" }; 
	    ORB orb = ORB.init(argv, null);

	    // Init POA
	    POA rootpoa = 
		POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
	    rootpoa.the_POAManager().activate();

	    // creer l'objet qui sera appele' depuis le serveur
	    client = new ClientDiscImpl();
	    org.omg.CORBA.Object ref = 
	        rootpoa.servant_to_reference(client);
	    client.chatter = ClientDiscHelper.narrow(ref); 
	    if (client == null) {
		System.out.println("Pb pour obtenir une ref sur le client");
		System.exit(1);
	    } 	

	    // contacter le serveur
	    String reference = "corbaname::"+args[0]+":1050#ServeurDisc";
	    org.omg.CORBA.Object obj = orb.string_to_object(reference); 	    

	    // obtenir reference sur l'objet distant
	    client.serveur = ServeurDiscHelper.narrow(obj);
	    if (client.serveur == null) {
		System.out.println("Pb pour contacter le serveur");
		System.exit(1);
	    } 
	    else System.out.println("Annonce du serveur : "+
				    client.serveur.ping());

	    // lancer l'ORB dans un thread
	    client.thread = new ThreadRun(orb);
	    client.thread.start();
	    client.loop();
	} catch (Exception e) {
	    System.out.println("ERROR : " + e) ;
	    e.printStackTrace(System.out);   
	} finally {
	    // shutdown
	    if (client != null) client.thread.shutdown();
	}
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
    public void shutdown() {
	orb.shutdown(false);   
    }
}





