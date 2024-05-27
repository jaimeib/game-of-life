GCC=gcc
MPICC=mpicc

################################################################

make: 0 1 2 3 4 GENERATOR TEST

################################################################
# 0. BASE
################################################################

0: BASE BASE_PRINT_RESULT BASE_PRINT BASE_OUTPUT BASE_DEBUG

BASE: BASE/src/life.c
	mkdir -p BASE/bin
	$(MPICC) -o BASE/bin/life BASE/src/life.c

BASE_PRINT_RESULT: BASE/src/life.c
	mkdir -p BASE/bin
	$(MPICC) -o BASE/bin/life_print_result BASE/src/life.c -DPRINT_RESULT

BASE_PRINT: BASE/src/life.c
	mkdir -p BASE/bin
	$(MPICC) -o BASE/bin/life_print BASE/src/life.c -DPRINT -DPRINT_RESULT

BASE_OUTPUT: BASE/src/life.c
	mkdir -p BASE/bin
	$(MPICC) -o BASE/bin/life_output BASE/src/life.c -DOUTPUT

BASE_DEBUG: BASE/src/life.c
	mkdir -p BASE/bin
	$(MPICC) -o BASE/bin/life_debug BASE/src/life.c -DDEBUG

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
# 3. LOAD BALANCING MPI
################################################################

3: LOAD_BALANCING_MPI LOAD_BALANCING_PRINT_RESULT_MPI LOAD_BALANCING_PRINT_MPI LOAD_BALANCING_OUTPUT_MPI LOAD_BALANCING_DEBUG_MPI

LOAD_BALANCING_MPI: PARALLEL/src/life.c PARALLEL/src/life.h
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_load_balancing_mpi PARALLEL/src/life.c -DLOAD_BALANCING

LOAD_BALANCING_PRINT_RESULT_MPI: PARALLEL/src/life.c PARALLEL/src/life.h
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_print_result_load_balancing_mpi PARALLEL/src/life.c -DPRINT_RESULT -DLOAD_BALANCING

LOAD_BALANCING_PRINT_MPI: PARALLEL/src/life.c PARALLEL/src/life.h
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_print_load_balancing_mpi PARALLEL/src/life.c -DPRINT -DPRINT_RESULT -DLOAD_BALANCING

LOAD_BALANCING_OUTPUT_MPI: PARALLEL/src/life.c PARALLEL/src/life.h
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_output_load_balancing_mpi PARALLEL/src/life.c -DOUTPUT -DLOAD_BALANCING

LOAD_BALANCING_DEBUG_MPI: PARALLEL/src/life.c PARALLEL/src/life.h
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_debug_load_balancing_mpi PARALLEL/src/life.c -DDEBUG -DLOAD_BALANCING

################################################################
# 4. LOAD BALANCING OPENMP
################################################################

4: LOAD_BALANCING_OPENMP LOAD_BALANCING_PRINT_RESULT_OPENMP LOAD_BALANCING_PRINT_OPENMP LOAD_BALANCING_OUTPUT_OPENMP LOAD_BALANCING_DEBUG_OPENMP

LOAD_BALANCING_OPENMP: PARALLEL/src/life.c PARALLEL/src/life.h
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_load_balancing_openmp PARALLEL/src/life.c -DLOAD_BALANCING -fopenmp

LOAD_BALANCING_PRINT_RESULT_OPENMP: PARALLEL/src/life.c PARALLEL/src/life.h
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_print_result_load_balancing_openmp PARALLEL/src/life.c -DPRINT_RESULT -DLOAD_BALANCING -fopenmp

LOAD_BALANCING_PRINT_OPENMP: PARALLEL/src/life.c PARALLEL/src/life.h
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_print_load_balancing_openmp PARALLEL/src/life.c -DPRINT -DPRINT_RESULT -DLOAD_BALANCING -fopenmp

LOAD_BALANCING_OUTPUT_OPENMP: PARALLEL/src/life.c PARALLEL/src/life.h
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_output_load_balancing_openmp PARALLEL/src/life.c -DOUTPUT -DLOAD_BALANCING -fopenmp

LOAD_BALANCING_DEBUG_OPENMP: PARALLEL/src/life.c PARALLEL/src/life.h
	mkdir -p PARALLEL/bin
	$(MPICC) -o PARALLEL/bin/life_debug_load_balancing_openmp PARALLEL/src/life.c -DDEBUG -DLOAD_BALANCING -fopenmp

################################################################
# COMPARE RESULTS:
# diff -qs DATA/out/life.out DATA/out/life-mpi.out
################################################################

################################################################
# GENERATOR:
################################################################
GENERATOR: GENERATOR/src/generator.c
	mkdir -p GENERATOR/bin
	$(GCC) -o GENERATOR/bin/generator GENERATOR/src/generator.c

################################################################
# TEST:
################################################################
TEST: TEST/src/test.c TEST/src/test.h
	mkdir -p TEST/bin
	$(MPICC) -o TEST/bin/test TEST/src/test.c

################################################################
# CLEAN:
################################################################
clean:
	rm -f BASE/bin/*
	rm -f PARALLEL/bin/*
	rm -f GENERATOR/bin/*
	rm -f TEST/bin/*
	rm -f DATA/out/*
