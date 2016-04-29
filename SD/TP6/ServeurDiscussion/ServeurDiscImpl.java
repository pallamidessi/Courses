import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import java.util.*;
import java.io.*;
import org.omg.CosNaming.*;
import Discussion.ClientDisc;
import Discussion.UserInfo;
import Discussion.ServeurDiscPOA;
import Discussion.ServeurDisc;
import Discussion.ServeurDiscHelper;

public class ServeurDiscImpl extends ServeurDiscPOA {
    Vector<UserInfo> chatters = new Vector<UserInfo>();

    public void login (String name, Discussion.ClientDisc c) {
	System.out.println(name+" entered");
	// A completer
    }

    public void logout (String name) {
	if (name == null) {
	    System.out.println("null name on logout: cannot remove chatter");
	    return;
	}
	UserInfo u_gone = null;
	Enumeration enume = null;
	synchronized (chatters) {
	    for (int i = 0; i < chatters.size(); i++) {
		UserInfo u = (UserInfo) chatters.elementAt(i);
		if (u.name.equals(name)) {
		    System.out.println(name+" left");
		    u_gone = u;
		    chatters.removeElementAt(i);
		    enume = chatters.elements();
		    break;
		}
	    }
	}
	if (u_gone == null || enume == null) {
	    System.out.println("no user by name of "+name+
			       " found: not removing chatter");
	    return;
	}
	while (enume.hasMoreElements()) {
	    UserInfo u = (UserInfo) enume.nextElement();
	    u.chatter.receiveExit(name);
	}
    }

    public void chat (String name, String message) {
	// A completer
    }   

    public String ping () {
	return ("Bienvenue sur le forum");
    }

    public static void main(String args[]) {
	if (args.length != 1) {
	    System.out.println("Usage : java ServeurDiscImpl"+
			       " <machineServeurDeNoms>");
	    return;
	}
	try{
	    String [] argv = {"-ORBInitialHost",args[0], 
			      "-ORBInitialPort","1050" }; 
 	    ORB orb = ORB.init(argv, null);
	    ServeurDiscImpl helloImpl = new ServeurDiscImpl();

	    // init POA
	    POA rootpoa =	       
		POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
	    rootpoa.the_POAManager().activate();
	  
	    org.omg.CORBA.Object ref = rootpoa.servant_to_reference(helloImpl);
	    ServeurDisc href = ServeurDiscHelper.narrow(ref);
	  
	    // inscription de l'objet au service de noms
	    org.omg.CORBA.Object objRef =
		orb.resolve_initial_references("NameService");
	    NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
	    NameComponent path[] = ncRef.to_name( "ServeurDisc" );
	    ncRef.rebind(path, href);

	    System.out.println("ServeurDisc ready and waiting ...");
	    orb.run();
	} catch (Exception e) {
	    System.err.println("ERROR: " + e);
	    e.printStackTrace(System.out);
	}
      
	System.out.println("ServeurDisc Exiting ..."); 
    }
}









