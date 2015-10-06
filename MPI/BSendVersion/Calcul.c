/*********************************************************************************/
/* Matrix product program with MPI on a virtual ring of processors               */
/* S. Vialle - October 2014                                                      */
/*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>
#include <cblas.h>

#include "main.h"
#include "Calcul.h"


/*-------------------------------------------------------------------------------*/
/* Sequential product of local matrixes (optimized seq product).                 */
/*-------------------------------------------------------------------------------*/
void OneLocalProduct(int step)
{
  int OffsetStepLigC;
  int i, j, k;

  // Set the number of OpenMP threads inside parallel regions
  omp_set_num_threads(NbThreads);

  // Compute the current step offset, in the MPI program, to access right C lines
  OffsetStepLigC = (Me * LOCAL_SIZE + step * LOCAL_SIZE) % SIZE;  

  switch (KernelId) {

  // Kernel 0 : Optimized code implemented by an application developer
  case 0 :
<<<<<<< Updated upstream
#pragma omp parallel
{
    #pragma omp for private(i)
=======
    #pragma omp parallel for 
>>>>>>> Stashed changes
    for (i = 0; i < LOCAL_SIZE; i++) {
      for (j = 0; j < LOCAL_SIZE; j++) {
        double accu[8];
        accu[0] = accu[1] = accu[2] = accu[3] =  accu[4] =  accu[5] =  accu[6] =  accu[7] = 0.0;
        for (k = 0; k < (SIZE/8)*8; k += 8) {
           accu[0] += A_Slice[i][k+0] * TB_Slice[j][k+0];
           accu[1] += A_Slice[i][k+1] * TB_Slice[j][k+1];
           accu[2] += A_Slice[i][k+2] * TB_Slice[j][k+2];
           accu[3] += A_Slice[i][k+3] * TB_Slice[j][k+3];
           accu[4] += A_Slice[i][k+4] * TB_Slice[j][k+4];
           accu[5] += A_Slice[i][k+5] * TB_Slice[j][k+5];
           accu[6] += A_Slice[i][k+6] * TB_Slice[j][k+6];
           accu[7] += A_Slice[i][k+7] * TB_Slice[j][k+7];
        }
        for (k = (SIZE/8)*8; k < SIZE; k++) {
           accu[0] += A_Slice[i][k] * TB_Slice[j][k];
        } 
        C_Slice[i+OffsetStepLigC][j] = accu[0] + accu[1] + accu[2] + accu[3] +
                                       accu[4] + accu[5] + accu[6] + accu[7];
      }
    }
}
    break;

  // Kernel 1 : Very optimized computing kernel implemented in a HPC library
  case 1 :
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                LOCAL_SIZE, LOCAL_SIZE, SIZE,
                1.0, &A_Slice[0][0], SIZE, 
                &B_Slice[0][0], LOCAL_SIZE,
                0.0, &C_Slice[OffsetStepLigC][0], LOCAL_SIZE);
    break;

  default :
    fprintf(stderr,"Error: kernel %d not implemented!\n",KernelId);
    exit(EXIT_FAILURE);
    break;
  }
}
