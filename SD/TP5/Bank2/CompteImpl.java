import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import Comptes.ComptePOA;
import java.io.*;

class CompteImpl extends ComptePOA{
  public float solde;
  
  public CompteImpl(){}
  
  public CompteImpl(int somme){
    solde=somme;
  }

  public void deposeBillets(float depot){
    solde+=depot;
  }

  public boolean retireBillets(float retrait){
    if (solde>=retrait) {
      solde-=retrait;
      return true;
    }
    return false;
  }

  public float afficheMontant(){
    return solde;
  } 

  public boolean virementCompteaCompte(float somme,Comptes.Compte destinataire){
    if (retireBillets(somme)) {
      destinataire.deposeBillets(somme);
      return true;
    }
      return false;
  }



}
