import java.rmi.server.UnicastRemoteObject ;
import java.rmi.RemoteException ;

class OpMatrice_impl extends UnicastRemoteObject implements OpMatrice{
	public OpMatrice_impl() throws RemoteException{
     super();
    
  }
   
 

	public int[][] multiplicationMatrice (int [][] a, int [][] b) throws RemoteException{
		int i,j,k;
		int dim=a.length;
    int result[][]=new int[dim][dim];
		
    if(a.length!=b.length || a[0].length!=b[0].length){
			throw new RemoteException();
		}
		else{

			for (i = 0; i < dim; i++) {
				for (j = 0; j < dim; j++) {
					for (k = 0; k < dim; k++) {
						result[i][j]+=a[i][k]*b[k][j];
					}
				}
			}
			
		}

		return result;
	}

	
}

