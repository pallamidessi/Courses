package paquetSuite;
import java.lang.Math.* ;

/**
 *@author Pallamidessi joseph
 *@version 1.0
 *@since 2013-02-18	
 **/
public abstract class Suite{
	protected int pas;
	protected int premier;

	protected Suite(int newPas,int newPremier){
		pas=newPas;	
		premier=newPremier;

	}
	/**
	 *Donne la valeur de la suite au rang n
	 *
	 *@param rang le rang auquel on veut la valeur de la suite
	 *@return la valeur
	 **/

	abstract public int valeurAuRangN(int rang);	

	/**
	 *Donne la somme de la suite au rang n
	 *
	 *@param rang le rang auquel on veut la somme de la suite
	 *@return la valeur
	 **/
	abstract public int sommeAuRangN(int rang);	
}
