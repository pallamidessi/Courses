/**
 *@author Pallamidessi joseph
 *@version 1.0
 *@since 2013-02-18	
 **/

//package paquetMain;
import paquetSuite.*;
public class Test{
	static public void main(String[] args){ 

		Suite s1;
		Suite s2;

		s1=new SuiteArithm(2,5); 
		s2=new SuiteGeom(2,5);
		try{
			System.out.println("Valeur au rang n pour s1:"+s1.valeurAuRangN(10)+"\n");
			System.out.println("Valeur au rang n pour s2:"+s2.valeurAuRangN(-1)+"\n");


			System.out.println("Somme au rang n pour s1:"+s1.sommeAuRangN(10)+"\n");
			System.out.println("Somme au rang n pour s2:"+s2.sommeAuRangN(10)+"\n");
		}catch(ArithmeticException e){
			System.out.println(e.getMessage());
		}
	}
}
