import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosNaming.*;
import java.io.*;

// Lancement du service de noms : tnameserv -ORBInitialPort 3000 &
// Lancement du serveur :         java Serveur localhost 3000 &
public class Serveur {
  public static void main(String args[]) { 
    if (args.length != 2) {
      System.out.println("Usage : java Serveur <machineServeurDeNoms> <port>");
      return;
    }
    try {
      org.omg.CORBA.Object poaobj;
      String [] argv = {"-ORBInitialHost",args[0], 
        "-ORBInitialPort",args[1]}; 
      ORB orb = ORB.init( argv, null ); 
      CompteImpl co1 = new CompteImpl(100);
      CompteImpl co2 = new CompteImpl(100);
      CompteImpl co3 = new CompteImpl(100);
      CompteImpl co4 = new CompteImpl(100);
      //initialiser le POA
      POA poa = POAHelper.narrow( orb.resolve_initial_references( "RootPOA" ));
      poa.the_POAManager().activate();
      // ETAPE 1
      org.omg.CORBA.Object obj = null;
      obj = orb.resolve_initial_references("NameService"); 
      if(obj == null) { 
        System.out.println("Ref nil sur NameService"); System.exit(1);
      }
      // ETAPE 2
      org.omg.CosNaming.NamingContext nc=NamingContextHelper.narrow(obj);
      // ETAPE 3

      //Agency name,context and binding to nc
      NameComponent[] agencyNameContext=new NameComponent[1];
      agencyNameContext[0]=new NameComponent("agence","");

      NamingContext agencyContext=nc.new_context();
      nc.rebind_context(agencyNameContext,agencyContext);

      //Client name,context and binding to agency
      NameComponent[] clientNameContext1=new NameComponent[1];
      clientNameContext1[0]=new NameComponent("Client1","");

      NameComponent[] clientNameContext2=new NameComponent[1];
      clientNameContext2[0]=new NameComponent("Client2","");

      NamingContext client1=agencyContext.new_context();
      agencyContext.rebind_context(clientNameContext1,client1);

      NamingContext client2=agencyContext.new_context();
      agencyContext.rebind_context(clientNameContext2,client2);

      //Client 1 compte name,object, and binding to client 1
      NameComponent[] firstClientCompte1=new NameComponent[1];
      firstClientCompte1[0]=new NameComponent("Compte1","");

      NameComponent[] firstClientCompte2=new NameComponent[1];
      firstClientCompte2[0]=new NameComponent("Compte2","");

      poaobj = poa.servant_to_reference( co1 );
      client1.bind(firstClientCompte1,poaobj);
      poaobj= poa.servant_to_reference(co2);
      client1.bind(firstClientCompte2,poaobj);


      //Client 2 compte name,object, and binding to client 2
      NameComponent[] secondClientCompte1=new NameComponent[1];
      secondClientCompte1[0]=new NameComponent("Compte1","");

      NameComponent[] secondClientCompte2=new NameComponent[1];
      secondClientCompte2[0]=new NameComponent("Compte2","");

      poaobj = poa.servant_to_reference( co3 );
      client2.bind(secondClientCompte1,poaobj);
      poaobj= poa.servant_to_reference(co4);
      client2.bind(secondClientCompte2,poaobj);


      // ETAPE 4
      System.out.println("Le serveur est pret ");
      orb.run();
    }
    catch( org.omg.CORBA.SystemException ex ) { ex.printStackTrace();}
    catch( org.omg.CORBA.UserException ex ) { ex.printStackTrace();}
  }
}





