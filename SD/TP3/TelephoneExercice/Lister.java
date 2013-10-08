import java.rmi.* ; 
import java.net.MalformedURLException ;

public class Lister{
	public static void main (String [] args){
		
		try {
			Annuaire b =(Annuaire) Naming.lookup("//localhost:2100/LAnnuaire" );
			b.listeNumero();
		}
		catch (NotBoundException re) { System.out.println(re) ; }
		catch (RemoteException re) { System.out.println(re) ; }
		catch (MalformedURLException e) { System.out.println(e) ; }
    }
}

