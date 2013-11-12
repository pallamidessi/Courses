import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import java.util.*;
import java.io.*;
import org.omg.CosNaming.*;
import HelloApp.HelloCallbackPOA;
import HelloApp.Hello;
import HelloApp.HelloHelper;
import HelloApp.HelloCallback;
import HelloApp.HelloCallbackHelper;

class HelloCallbackImpl extends HelloCallbackPOA {
    public void callback(String message) {
	System.out.println(message);   }}

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
	orb.shutdown(false);   }}

public class HelloClient {
    public static void main(String args[]) {
	try{
	    ORB orb = ORB.init(args, null);
	    org.omg.CORBA.Object objRef = 
		orb.resolve_initial_references("NameService");
	    NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
	    String name = "Hello";

	    // obtenir reference sur l'objet distant
	    Hello helloImpl = HelloHelper.narrow(ncRef.resolve_str(name));
	    // Init POA
	    POA rootpoa = 
		POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
	    rootpoa.the_POAManager().activate();

	    // creer l'objet qui sera appele' depuis le serveur
	    HelloCallbackImpl helloCallbackImpl = new HelloCallbackImpl();
	    org.omg.CORBA.Object ref = 
	        rootpoa.servant_to_reference(helloCallbackImpl);
	    HelloCallback hcbref = HelloCallbackHelper.narrow(ref); 
	
	    // lancer l'ORB dans un thread
	    ThreadRun thread = new ThreadRun(orb);
	    thread.start();

	    // declenche la methode sur l'objet distant
	    System.out.println(helloImpl.sayHello(hcbref,"premier affichage"));

	    // shutdown
	    helloImpl.shutdown();
	    thread.shutdown();
	    
	} catch (Exception e) {
	    System.out.println("ERROR : " + e) ;
	    e.printStackTrace(System.out);	}}}
