import org.omg.CORBA.*;
import org.omg.CosNaming.*;
import java.io.*;
// Exemple d'utilisation : java Client1 localhost 3000

public class Client2 {

  public static void print_context(NamingContext nc){
    int i,j;
    BindingListHolder bList=new BindingListHolder();
    BindingIteratorHolder bIterator=new BindingIteratorHolder();

    nc.list(10,bList,bIterator);

    for (i = 0; i < bList.value.length; i++) {
      for (j = 0; j < bList.value[i].binding_name.length; j++) {
        System.out.println(bList.value[i].binding_name[j].id);
      }
    }

  }


  public static void main(String args[]) { 

    try {
      if (args.length != 3) {
        System.out.println("Usage : java Client1 <nom agence> <machineServeurDeNoms> <port>");
        return;
      }
      String[] adress={"-ORBInitialHost",args[1],"-ORBInitialPort",args[2]};
      ORB orb = ORB.init( adress, null ); 
      org.omg.CORBA.Object o= orb.resolve_initial_references("NameService");

      if (o==null) {
        System.out.println("Erreur:Pas de contexte de ce nom !");
        return ;
      }

      NamingContext nc= NamingContextHelper.narrow(o);
      print_context(nc);

      NameComponent[] agenceName=new NameComponent[1];
      agenceName[0]=new NameComponent(args[0],"");

      try{
        o=nc.resolve(agenceName);
      }catch(Exception e){
        System.out.println("Erreur:Pas de contexte de ce nom !");
        return;
      }

      if (o==null) {
        System.out.println("Erreur:Pas de contexte de ce nom !");
        return ;
      }

      NamingContext agence= NamingContextHelper.narrow(o);
      print_context(agence);
    }
    //catch( org.omg.CORBA.SystemException ex ) { ex.printStackTrace();}
    catch( Exception ex ) { ex.printStackTrace();}
  }
}
