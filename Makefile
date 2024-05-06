GCC=gcc
MPICC=mpicc

################################################################

make: 0 1

################################################################
# 0. BASE
################################################################

0: BASE BASE_PRINT BASE_DEBUG

BASE: 0.BASE/src/life.c
	$(GCC) -o 0.BASE/build/life 0.BASE/src/life.c

BASE_PRINT: 0.BASE/src/life.c
	$(GCC) -o 0.BASE/build/life_print 0.BASE/src/life.c -DPRINT

BASE_DEBUG: 0.BASE/src/life.c
	$(GCC) -o 0.BASE/build/life_debug 0.BASE/src/life.c -DDEBUG

################################################################
# 1. MPI
################################################################

1: MPI MPI_PRINT MPI_DEBUG

MPI: 1.MPI/src/life.c
	$(MPICC) -o 1.MPI/build/life 1.MPI/src/life.c

MPI_PRINT: 1.MPI/src/life.c
	$(MPICC) -o 1.MPI/build/life_print 1.MPI/src/life.c -DPRINT

MPI_DEBUG: 1.MPI/src/life.c
	$(MPICC) -o 1.MPI/build/life_debug 1.MPI/src/life.c -DDEBUG

################################################################

clean:
	rm -f 0.BASE/build/life 0.BASE/build/life_print 0.BASE/build/life_debug
	rm -f 1.MPI/build/life 1.MPI/build/life_print 1.MPI/build/life_debug
