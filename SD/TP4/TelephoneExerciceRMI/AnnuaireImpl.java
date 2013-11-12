import java.rmi.*; 
import java.util.*; 
import java.rmi.server.UnicastRemoteObject;

public class AnnuaireImpl extends AnnuairePOA{
    Hashtable numeros = new Hashtable();
    
  public AnnuaireImpl() {
	  numeros.put("tintin", "06 76 70 80 09");
	  numeros.put("milou", "06 50 40 36 76");
	  numeros.put("tournesol", "06 07 33 72 06");
  }
    
  public void listerNoms (org.omg.CORBA.StringHolder liste) {
    String res = " " ;
    Iterator it = numeros.keySet().iterator();
    
    while (it.hasNext()) {
        String nom = (String) it.next();
        res += nom + " : " + numeros.get(nom) + " \n " ;
    }

    liste.value=res;
  }
    
  public void chercheNom(String nom,org.omg.CORBA.StringHolder numero) {    
	  String resultat = (String) numeros.get(nom); 
	  if (resultat == null) resultat = "Nom inconnu : " + nom;
    numero.value=resultat;
  }
    
  public void enleveNom(String nom) {    
	  numeros.remove(nom); 
  }
    
  public void ajouteNom(String nom, String num) { 
    String resultat = (String) numeros.put(nom,num); 
  }
    
}
