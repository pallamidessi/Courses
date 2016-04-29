/*********************************************************************************/
/* Matrix product program with MPI on a virtual ring of processors               */
/* S. Vialle - October 2014                                                      */
/*********************************************************************************/

#ifndef __INIT__
#define __INIT__


void ProcessorInit(void);                        // Processor init
void LocalMatrixInit(void);                      // Data init

void usage(int ExitCode, FILE *std);             // Cmd line parsing and usage
void CommandLineParsing(int argc, char *argv[]); 

void PrintResultsAndPerf(double megaflops, double d1,double d2); // Res printing


#endif

// END
