GCC=gcc
MPICC=mpicc

################################################################

make: 0 1 2

################################################################
# 0. BASE
################################################################

0: BASE BASE_PRINT_RESULT BASE_PRINT BASE_OUTPUT BASE_DEBUG

BASE: BASE/src/life.c
	mkdir -p BASE/bin
	$(GCC) -o BASE/bin/life BASE/src/life.c

BASE_PRINT_RESULT: BASE/src/life.c
	mkdir -p BASE/bin
	$(GCC) -o BASE/bin/life_print_result BASE/src/life.c -DPRINT_RESULT

BASE_PRINT: BASE/src/life.c
	mkdir -p BASE/bin
	$(GCC) -o BASE/bin/life_print BASE/src/life.c -DPRINT -DPRINT_RESULT

BASE_OUTPUT: BASE/src/life.c
	mkdir -p BASE/bin
	$(GCC) -o BASE/bin/life_output BASE/src/life.c -DOUTPUT

BASE_DEBUG: BASE/src/life.c
	mkdir -p BASE/bin
	$(GCC) -o BASE/bin/life_debug BASE/src/life.c -DDEBUG

################################################################
# 1. MPI
################################################################

1: MPI MPI_PRINT_RESULT MPI_PRINT MPI_OUTPUT MPI_DEBUG

MPI: PARALLEL/src/life.c
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_mpi PARALLEL/src/life.c

MPI_PRINT_RESULT: PARALLEL/src/life.c
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_print_result_mpi PARALLEL/src/life.c -DPRINT_RESULT

MPI_PRINT: PARALLEL/src/life.c
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_print_mpi PARALLEL/src/life.c -DPRINT -DPRINT_RESULT

MPI_OUTPUT: PARALLEL/src/life.c
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_output_mpi PARALLEL/src/life.c -DOUTPUT

MPI_DEBUG: PARALLEL/src/life.c
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_debug_mpi PARALLEL/src/life.c -DDEBUG

################################################################
# 2. OpenMP
################################################################

2: OPENMP OPENMP_PRINT_RESULT OPENMP_PRINT OPENMP_OUTPUT OPENMP_DEBUG

OPENMP: PARALLEL/src/life.c
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_openmp PARALLEL/src/life.c -fopenmp

OPENMP_PRINT_RESULT: PARALLEL/src/life.c
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_print_result_openmp PARALLEL/src/life.c -DPRINT_RESULT -fopenmp

OPENMP_PRINT: PARALLEL/src/life.c
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_print_openmp PARALLEL/src/life.c -DPRINT -DPRINT_RESULT -fopenmp

OPENMP_OUTPUT: PARALLEL/src/life.c
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_output_openmp PARALLEL/src/life.c -DOUTPUT -fopenmp

OPENMP_DEBUG: PARALLEL/src/life.c
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_debug_openmp PARALLEL/src/life.c -DDEBUG -fopenmp

################################################################
# COMPARE RESULTS:
# diff -qs DATA/out/life.out DATA/out/life-mpi.out
################################################################

clean:
	rm -f BASE/bin/*
	rm -f PARALLEL/bin/*
