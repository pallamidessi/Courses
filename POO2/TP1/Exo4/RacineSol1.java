
public class RacineSol1 extends Racines{
	
	private double valRacine= -getCoeff(1)/Math.sqrt(getDiscr());

	public RacineSol1(double a,double b,double c,double delta){
			super(a,b,c,delta);
	}
	
	public void calculRacines(){
		System.out.println("Le determinant est egale a 0, il n'y a donc une racine possible /n");
		System.out.println("Racine : " +valRacine + "\n");
	}	
	
	public int nbRacines(){
		System.out.println("il y a une racine possible ");
		return 1;
	}
	
	public double valeurRacine(int i){
		System.out.println("Il y a une valeur valeur possible (racine double)");
		return valRacine;
	}
}
