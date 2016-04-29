class CompteImpl extends ComptePOA{
  public float solde;

  void deposeBillets(float depot){
    solde+=depot;
  }

  boolean retireBillets(float retrait){
    if (solde>=retrait) {
      solde-=retrait
      return true;
    }
    return false;
  }

  float afficheMontant(){
    return solde;
  } 

  boolean virementCompteaCompte(float somme, CompteImpl destinataire){
    if (retireBillet(somme)) {
      destinataire.deposeBillets(somme);
      return true;
    }
      return false;
  }



}
