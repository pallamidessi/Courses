import OpMatricePackage.*;
class OpMatriceImpl extends OpMatricePOA {
 

	public int[][] multiplicationMatrice (int [][] a, int [][] b) throws illegalMatriceMultiplication{
		int i,j,k;
		int dim1=a.length;
		int dim2=b[0].length;
    int dim3=b.length;

    int result[][]=new int[dim1][dim2];
		
    if(a[0].length!=b.length){
			throw new illegalMatriceMultiplication();
		}
		else{
			
      for (i = 0; i < dim1; i++) {
				for (j = 0; j < dim2; j++) {
					for (k = 0; k < dim3; k++) {
						result[i][j]+=a[i][k]*b[k][j];
					}
				}
			}

		}

		return result;
	}

	
}

