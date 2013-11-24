import org.omg.CORBA.*;
import org.omg.PortableServer.*;



public class Serveur {

  public static void main(String[] args) {
    try {
      //init ORB
      ORB orb = ORB.init( args, null );

      IcarreImpl myobj = new IcarreImpl();      

      //create and active POA

      POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootpoa.the_POAManager().activate();
      org.omg.CORBA.Object poaobj = rootpoa.servant_to_reference( myobj ) ;


      String ior = orb.object_to_string( poaobj );

      System.out.println( ior );

      orb.run();
    }
    catch( org.omg.CORBA.SystemException ex ) { ex.printStackTrace();}
    catch( org.omg.CORBA.UserException ex ) { ex.printStackTrace();}
  }
}
