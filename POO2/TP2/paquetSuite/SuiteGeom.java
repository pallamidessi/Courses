
package paquetSuite;

/**
 *@author Pallamidessi joseph
 *@version 1.0
 *@since 2013-02-18	
 **/
public class SuiteGeom extends Suite{
	
	public SuiteGeom(int newPas,int newPremier){
		super(newPas,newPremier);
	}

	public int valeurAuRangN(int rang){
		if(rang>=0)
			return premier*((int) Math.pow(pas,rang));
		else
			throw new ArithmeticException("Valeur inferieur a 0 !");
	}
	
	public int sommeAuRangN(int rang){
		
		if(rang<0)
			throw new ArithmeticException("Valeur inferieur a 0 !");

		if(pas==1)
			return premier*rang;
		else
			return premier*(1-((int) Math.pow((1-pas),rang)))/(1-pas);
	}
}

