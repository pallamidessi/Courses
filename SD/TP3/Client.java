import java.rmi.* ; 
import java.net.MalformedURLException ; 




public class Client {
   
	public static void print_matrice(int[][] m){
		int dim=m.length;
    int i,j;

		for (i = 0; i < dim; i++) {
			for (j = 0; j < dim; j++) {
				System.out.print(m[i][j]+" ");
			}
			System.out.println();
		}

	}
	
	 
	public static void main(String [] args) {
	if (args.length != 1) {
	    System.out.println("Usage : java Client <machine du Serveur:port du rmiregistry>");
	    System.exit(0);
	}
	try {
	    OpMatrice m =(OpMatrice) Naming.lookup("//"+args[0]+"/OpMatrice" );
      int[][] a = { {1, 0, 0}, {0, 2, 0}, {0, 0, 3} };
      int[][] b = { {1, 2, 3}, {1, 2, 3}, {1, 2, 3} };
			
	    System.out.println("Soit la matrice A:");
			Client.print_matrice(a);
	    System.out.println("Soit la matrice B:");
			Client.print_matrice(b);

			int[][] result= m.multiplicationMatrice(a,b);
	    
			System.out.println("Le résultat de AB:");
			print_matrice(result);


	}
	catch (NotBoundException re) { System.out.println(re) ; }
	catch (RemoteException re) { System.out.println(re) ; }
	catch (MalformedURLException e) { System.out.println(e) ; }
    }
}
