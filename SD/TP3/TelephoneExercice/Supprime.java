import java.rmi.* ; 
import java.net.MalformedURLException ;

public class Supprime{
  public static void main (String [] args){
    if (args.length != 1) {
      System.out.println("Usage : java Client <machine du Serveur:port du rmiregistry>");
      System.exit(0);
    }
    try {
      Annuaire b =(Annuaire) Naming.lookup("//localhost:2100/LAnnuaire" );
      b.removeNumero(args[0]);
    }
    catch (NotBoundException re) { System.out.println(re) ; }
    catch (RemoteException re) { System.out.println(re) ; }
    catch (MalformedURLException e) { System.out.println(e) ; }
  }
}

