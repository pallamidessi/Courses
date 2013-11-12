import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import java.util.*;
import java.io.*;
import org.omg.CosNaming.*;
import HelloApp.HelloPOA;
import HelloApp.Hello;
import HelloApp.HelloCallback;
import HelloApp.HelloHelper;

class HelloImpl extends HelloPOA {
  private ORB orb;
  public void setORB(ORB orb_val) {
    orb = orb_val; 
  }
  public String sayHello (HelloCallback objRef, String message) {
    objRef.callback(message);
    return "second affichage";
  }
  public void shutdown() {
    orb.shutdown(false); }
}

public class HelloServeur {
  public static void main(String args[]) {
    try{
      ORB orb = ORB.init(args, null);

      HelloImpl helloImpl = new HelloImpl();
      helloImpl.setORB(orb); 

      // init POA
      POA rootpoa =	       
      POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootpoa.the_POAManager().activate();

      org.omg.CORBA.Object ref = rootpoa.servant_to_reference(helloImpl);
      Hello href = HelloHelper.narrow(ref);
	  
      // inscription de l'objet au service de noms
      org.omg.CORBA.Object objRef =
          orb.resolve_initial_references("NameService");
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
      NameComponent path[] = ncRef.to_name( "Hello" );
      ncRef.rebind(path, href);

      System.out.println("HelloServeur ready and waiting ...");
      orb.run();
    } 
	
    catch (Exception e) {
        System.err.println("ERROR: " + e);
        e.printStackTrace(System.out);
      }
	  
      System.out.println("HelloServeur Exiting ..."); }
}
