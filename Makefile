GCC=gcc
MPICC=mpicc

################################################################

make: 0 1

################################################################
# 0. BASE
################################################################

0: BASE BASE_PRINT BASE_DEBUG

BASE: 0.BASE/src/life.c
	mkdir -p 0.BASE/bin
	$(GCC) -o 0.BASE/bin/life 0.BASE/src/life.c

BASE_PRINT: 0.BASE/src/life.c
	mkdir -p 0.BASE/bin
	$(GCC) -o 0.BASE/bin/life_print 0.BASE/src/life.c -DPRINT

BASE_DEBUG: 0.BASE/src/life.c
	mkdir -p 0.BASE/bin
	$(GCC) -o 0.BASE/bin/life_debug 0.BASE/src/life.c -DDEBUG

################################################################
# 1. MPI
################################################################

1: MPI MPI_PRINT_RESULT MPI_PRINT MPI_OUTPUT MPI_DEBUG

MPI: 1.MPI/src/life.c
	mkdir -p 1.MPI/bin
	$(MPICC) -o 1.MPI/bin/life 1.MPI/src/life.c

MPI_PRINT_RESULT: 1.MPI/src/life.c
	mkdir -p 1.MPI/bin
	$(MPICC) -o 1.MPI/bin/life_print_result 1.MPI/src/life.c -DPRINT_RESULT

MPI_PRINT: 1.MPI/src/life.c
	mkdir -p 1.MPI/bin
	$(MPICC) -o 1.MPI/bin/life_print 1.MPI/src/life.c -DPRINT -DPRINT_RESULT

MPI_OUTPUT: 1.MPI/src/life.c
	mkdir -p 1.MPI/bin
	$(MPICC) -o 1.MPI/bin/life_output 1.MPI/src/life.c -DOUTPUT

MPI_DEBUG: 1.MPI/src/life.c
	mkdir -p 1.MPI/bin
	$(MPICC) -o 1.MPI/bin/life_debug 1.MPI/src/life.c -DDEBUG

################################################################

clean:
	rm -f 0.BASE/bin/life 0.BASE/bin/life_print 0.BASE/bin/life_debug
	rm -f 1.MPI/bin/life 1.MPI/bin/life_print 1.MPI/bin/life_debug
