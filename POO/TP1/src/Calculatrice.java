import java.lang.Math;
import java.io.*;

public class Calculatrice {
	public static void main(String[] args) throws IOException{
		String line;
		BufferedReader br;
		
		br=new BufferedReader(new InputStreamReader(System.in));
		System.out.println("1ere operande :");
		line=br.readLine();
		int op1=Integer.parseInt(line);
		
		
		System.out.println("operateur :");
		line=br.readLine();
		char operateur=(line.charAt(0));
		
		
		System.out.println("2eme operande :");
		line=br.readLine();
		int op2=Integer.parseInt(line);
		
		
		
	
			
			switch (operateur) {
				case '+' : System.out.println("Le resultat  est " + op1 + "+" + op2 + "=" + (op1 + op2));
					break;
				case '-' : System.out.println("Le resultat  est " + op1 + "-" + op2 + "=" + (op1 - op2));
						break;
				case'*' : System.out.println("Le resultat  est " + op1 + "*" + op2 + "=" + (op1 * op2));
					break;
				case '/' :System.out.println("Le resultat  est " + op1 + "/" + op2 + "=" + ((double)op1 / (double)op2));
					break;
				case '^' :System.out.println("Le resultat  est " + op1 + "^" + op2 + "=" + (Math.pow((double) op1 ,(double) op2)));
					break;
				case '%' :System.out.println("Le resultat  est " + op1 + "%" + op2 + "=" + (op1 % op2));
					break;
				default : System.out.println("operateur inconnu ...");
			} // switch
		
	} // method main
} // class