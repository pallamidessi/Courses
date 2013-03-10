
public class RacineSol2 extends Racines{
	
	private double valRacine1= (-getCoeff(1)+Math.sqrt(getDiscr()))/2*getCoeff(2);
	private double valRacine2= (-getCoeff(1)-Math.sqrt(getDiscr()))/2*getCoeff(2);

	public RacineSol2(double a,double b,double c,double delta){
			super(a,b,c,delta);
	}
	
	public void calculRacines(){
		System.out.println("Le determinant est superieur  a 0, il y a donc deux racines possible /n");
	}	
	
	public int nbRacines(){
		System.out.println("il y a deux racine possible ");
		return 2;
	}
	
	public double valeurRacine(int i){
		if(i==1)
			return valRacine1;
		else if(i==2)
			return valRacine2;
		else
			return 0.0;
	}
}
