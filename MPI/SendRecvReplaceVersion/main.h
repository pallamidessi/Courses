/*********************************************************************************/
/* Matrix product program with MPI on a virtual ring of processors               */
/* S. Vialle - October 2014                                                      */
/*********************************************************************************/

#ifndef __MAIN__
#define __MAIN__


/*-------------------------------------------------------------------------------*/
/* CONSTANTS.                                                                    */
/*-------------------------------------------------------------------------------*/

//#define SIZE              1024
#define SIZE              (3*5*7*32)

//#define LOCAL_SIZE        (SIZE/32)             // 1 MPI process
//#define LOCAL_SIZE        (SIZE/32)             // 2 MPI processes
#define LOCAL_SIZE        (SIZE/32)             // 4 MPI processes


// Constant for computation configuration 
#define DEFAULT_NB_THREADS  1                    /* Default: run 1 thread        */
#define DEFAULT_KERNEL_ID   1                    /* Default: run user kernel     */


/*-------------------------------------------------------------------------------*/
/* Global variable declarations.                                                 */
/*-------------------------------------------------------------------------------*/

/* Matrixes: C = A.B                                                             */
/* We use the Transposed B matrix, in place of B, to improve cache memory usage. */
extern double A_Slice[LOCAL_SIZE][SIZE];       /* Slices of matrixes (C = AxB) */
extern double B_Slice[SIZE][LOCAL_SIZE];
extern double TB_Slice[LOCAL_SIZE][SIZE];
extern double C_Slice[SIZE][LOCAL_SIZE];

/* Global variables for the management of the parallelisation.                   */
/* Need to be initialized dynamically: set to dummy values for the moment.       */
extern int Me;                                  /* Processor coordinates.       */
extern int NbPE;                                /* Number of processors         */

/* Global variables for the management of the result and performance printing.   */
extern int PrinterPE;      /* Processor hosting the central elt of the C matrix.*/
extern int PrintedElt_i;   /* Coordinates of the central elt of the C matrix in */ 
extern int PrintedElt_j;   /* its host processor.                               */

/* Global variables to control OpenMP and kernel computations.                   */
extern int NbThreads;                           /* Nb of OpenMP threads         */
extern int KernelId;                            /* Kernel Id                    */


/*-------------------------------------------------------------------------------*/
/* Global functions.                                                             */
/*-------------------------------------------------------------------------------*/
void ComputationAndCirculation();
void OneStepCirculation(int step);
int main(int argc, char *argv[]);


#endif

// END
