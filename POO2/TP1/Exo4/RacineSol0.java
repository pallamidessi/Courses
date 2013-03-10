
public class RacineSol0 extends Racines{
	
	public RacineSol0(double a,double b,double c,double delta){
			super(a,b,c,delta);
	}
	
	public void calculRacines(){
		System.out.println("Le determinant est inferieur a 0, il n'y a donc pas de resultat /n");
	}	
	
	public int nbRacines(){
		System.out.println("il n'y a pas de racine possible ");
		return 0;
	}
	
	public double valeurRacine(int i){
		System.out.println("Erreur :il n'y a pas de racine possible ");
		return 0.0;
	}
}
