CC=mpicc
CFLAGS= -O3 -fopenmp -funroll-loops #-I/opt/atlas/include
LDFLAGS= #-L/opt/atlas/lib
SOURCES= Init.c Calcul.c main.c 
EXECNAME= MatrixProduct

all:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(EXECNAME) $(SOURCES) -lm -lcblas -latlas

clean:
	rm -f *.o core $(EXECNAME)



