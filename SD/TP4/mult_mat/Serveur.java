import org.omg.CORBA.*;
import org.omg.PortableServer.*;



public class Serveur {

  public static void main(String[] args) {
    try {
      //init ORB
      ORB orb = ORB.init( args, null );

      OpMatriceImpl myobj = new OpMatriceImpl();      

      //createPOA
      POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));

      //Activate the POA manager
      rootpoa.the_POAManager().activate();

      //reference to object from the manager
      org.omg.CORBA.Object poaobj = rootpoa.servant_to_reference( myobj ) ;


      String ior = orb.object_to_string( poaobj );

      System.out.println( ior );

      orb.run();
    }
    catch( org.omg.CORBA.SystemException ex ) { ex.printStackTrace();}
    catch( org.omg.CORBA.UserException ex ) { ex.printStackTrace();}
  }
}
