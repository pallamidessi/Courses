import java.rmi.*; 
import java.util.*; 
import java.rmi.server.UnicastRemoteObject;

public class AnnuaireImpl 
extends UnicastRemoteObject
implements Annuaire {
  Hashtable numeros = new Hashtable();

  public AnnuaireImpl() throws RemoteException {
    super();
    numeros.put("tintin", "06 76 70 80 09");
    numeros.put("milou", "06 50 40 36 76");
    numeros.put("tournesol", "06 07 33 72 06");
  }

  public String chercheNom(String nom) throws RemoteException {    
    String resultat = (String) numeros.get(nom); 
    if (resultat == null) resultat = "Nom inconnu : " + nom;
    return resultat;
  }

  public void putNumero(String name,String num) throws RemoteException{
    numeros.put(name,num);
    return ;
  }

  public void removeNumero(String name) throws RemoteException{
    numeros.remove(name);
    return ;
  }

  public void listeNumero() throws RemoteException{
    Iterator it=numeros.keySet().iterator();

    while(it.hasNext()){
      System.out.println("Annuaire: " + it.next().toString());
    }
    return ;
  }

  public static void main(String[] args) {
    try {
      AnnuaireImpl annuaire = new AnnuaireImpl ();
      Naming.rebind("rmi://localhost:"+args[0]+"/LAnnuaire", annuaire);
      System.out.println("Annuaire en service");
    } catch (Exception e) {
      System.out.println("AnnuaireImpl : " + e.getMessage());
      e.printStackTrace();
    }
  }
}
