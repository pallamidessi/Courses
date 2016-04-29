package paquetSuite;

/**
 *@author Pallamidessi joseph
 *@version 1.0
 *@since 2013-02-18	
 **/
public class SuiteArithm extends Suite{
	
	public SuiteArithm(int newPas,int newPremier){
		super(newPas,newPremier);
	}
	
	public int valeurAuRangN(int rang){
		if(rang>=0)
			return premier+(rang-1)*pas;
		else
			throw new ArithmeticException("Valeur inferieur a 0 !");

	}
	
	public int sommeAuRangN(int rang){
		if(rang>=0)
			return rang*((premier+valeurAuRangN(rang))/2);
		else
			throw new ArithmeticException("Valeur inferieur a 0 !");
	}
}

	
	
	
	
	
	
	
