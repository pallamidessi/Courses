import org.omg.CORBA.*;
import org.omg.CosNaming.*;
import java.io.*;
// Exemple d'utilisation : java Client1 localhost 3000
public class Client {
  public static void main(String args[]) { 
    try {
      if (args.length != 2) {
        System.out.println("Usage : java Client1 <machineServeurDeNoms> <port>");
        return;
      }
      ORB orb = ORB.init( new String[0], null ); 
      String linkCo1="corbaname::" + args[0] + ":" + args[1] + "#agence/Client1/Compte1" ;
      String linkCo2="corbaname::" + args[0] + ":" + args[1] + "#agence/Client1/Compte2" ;
      String linkCo3="corbaname::" + args[0] + ":" + args[1] + "#agence/Client2/Compte1" ;
      String linkCo4="corbaname::" + args[0] + ":" + args[1] + "#agence/Client2/Compte2" ;

      org.omg.CORBA.Object obj1 = orb.string_to_object(linkCo1) ; 
      org.omg.CORBA.Object obj2 = orb.string_to_object(linkCo2) ; 
      org.omg.CORBA.Object obj3 = orb.string_to_object(linkCo3) ; 
      org.omg.CORBA.Object obj4 = orb.string_to_object(linkCo4) ; 

      if (obj1 ==null || obj2==null || obj3==null|| obj4==null) {
        System.out.println("Erreur d'accès au objet corba");
      }

      Comptes.Compte co1 = Comptes.CompteHelper.narrow(obj1);
      Comptes.Compte co2 = Comptes.CompteHelper.narrow(obj2);
      Comptes.Compte co3 = Comptes.CompteHelper.narrow(obj3);
      Comptes.Compte co4 = Comptes.CompteHelper.narrow(obj4);

      if (co1 ==null || co2==null || co3==null|| co4==null) {
        System.out.println("Erreur de récupération des objets");
      }
      System.out.println("Ajout de 50");
      co1.deposeBillets((float)50.);
      co2.deposeBillets((float)50.);
      co3.deposeBillets((float)50.);
      co4.deposeBillets((float)50.);
      System.out.println("somme co1 : "+co1.afficheMontant()+", somme co2: "+co2.afficheMontant()+", somme co3: "+co3.afficheMontant()+", somme co4: "+co4.afficheMontant());

      System.out.println("Retrait de 75");
      co1.retireBillets((float)75.);
      co2.retireBillets((float)75.);
      co3.retireBillets((float)75.);
      co4.retireBillets((float)75.);
      System.out.println("somme co1 : "+co1.afficheMontant()+", somme co2: "+co2.afficheMontant()+", somme co3: "+co3.afficheMontant()+", somme co4: "+co4.afficheMontant());

      System.out.println("Virement de 50 de co1 vers co3 et de co4 vers co2");
      co1.virementCompteaCompte(50,co3);
      co4.virementCompteaCompte(50,co2);
      System.out.println("somme co1 : "+co1.afficheMontant()+", somme co2: "+co2.afficheMontant()+", somme co3: "+co3.afficheMontant()+", somme co4: "+co4.afficheMontant());
    }
    //catch( org.omg.CORBA.SystemException ex ) { ex.printStackTrace();}
    catch( Exception ex ) { ex.printStackTrace();}
  }
}
