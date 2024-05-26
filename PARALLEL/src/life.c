/************************************************************/
/* LIFE: Conway Game of Life                                */
/************************************************************/
/*                                                          */
/* Nombre: Jaime Iglesias Blanco                            */
/* Práctica:                                                */
/* Fecha: 					                                */
/*                                                          */
/* Usage: mpirun -np [N] life [<input file>] [size] [steps] */
/*                                                          */
/************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <mpi.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#ifdef LOAD_BALANCING
#include "life.h"
#endif

#define TWO_ADJACENT_ROWS 2
#define ONE_ADJACENT_ROWS 1

typedef unsigned char cell_t;

// Descomentar esta línea para ver el tablero en pantalla. Sólo para el fichero life.in
// #define DEBUG 1

/**
 * @brief Allocate memory for the board
 *
 * @param size_rows size of the board
 * @param size_cols size of the board
 * @return cell_t** pointer to the board
 */
cell_t *allocate_board(int size_rows, int size_cols)
{
	cell_t *board = (cell_t *)malloc(size_rows * size_cols * sizeof(cell_t));
	if (board == NULL)
	{
		printf("Error: Cannot allocate memory for the board\n");
		exit(1);
	}
	return board;
}

/**
 * @brief Free the memory allocated for the board
 *
 * @param board board to free
 */
void free_board(cell_t *board)
{
	free(board);
}

/**
 * @brief Return the number of on cells adjacent to the i,j cell
 *
 * @param board board to check
 * @param size_rows size of the board rows
 * @param size_cols size of the board columns
 * @param i row of the cell
 * @param j column of the cell
 * @return int number of on cells adjacent to the i,j cell
 */
int adjacent_to(cell_t *board, int size_rows, int size_cols, int i, int j)
{
	int k, l, count = 0;

	int sk = (i > 0) ? i - 1 : i;
	int ek = (i + 1 < size_rows) ? i + 1 : i;
	int sl = (j > 0) ? j - 1 : j;
	int el = (j + 1 < size_cols) ? j + 1 : j;

	for (k = sk; k <= ek; k++)
		for (l = sl; l <= el; l++)
			count += board[k * size_cols + l];
	count -= board[i * size_cols + j];

	return count;
}

/**
 * @brief Apply the rules of the game of life to the board
 *
 * @param board board to apply the rules
 * @param newboard new board to store the result
 * @param size_cols size of the board
 * @param start start row
 * @param end end row
 */
void play(cell_t *board, cell_t *newboard, int size_rows, int size_cols, int start, int end)
{
	int i, j, a;
	/* for each cell, apply the rules of Life */

#ifdef _OPENMP
#pragma omp parallel for private(i, j, a) schedule(static)
#endif
	for (i = start; i < end; i++)
		for (j = 0; j < size_cols; j++)
		{
			a = adjacent_to(board, size_rows, size_cols, i, j);
			/* If a == 2, the cell keeps the same */
			if (a == 2)
				newboard[i * size_cols + j] = board[i * size_cols + j];
			/* If a == 3, then cell lives */
			if (a == 3)
				newboard[i * size_cols + j] = 1;
			/* If a < 2, then cell dies */
			if (a < 2)
				newboard[i * size_cols + j] = 0;
			/* If a > 3, then cell dies */
			if (a > 3)
				newboard[i * size_cols + j] = 0;
		}
}

/**
 * @brief Print the board in the screen
 *
 * @param board board to print
 * @param size size of the board (size x size)
 */
void print(cell_t *board, int size)
{
	int i, j;
	/* for each row */
	for (j = 0; j < size; j++)
	{
		/* print each column position... */
		for (i = 0; i < size; i++)
			printf("%c", board[i * size + j] ? 'x' : ' ');
		/* followed by a carriage return */
		printf("\n");
	}
}

/**
 * @brief Read the board from a file
 *
 * @param f file to read
 * @param board board to store the result
 * @param size size of the board (size x size)
 */
void read_file(FILE *f, cell_t *board, int size)
{
	int i, j;
	char *s = (char *)malloc(size + 10);
	char c;
	for (j = 0; j < size; j++)
	{
		/* get a string */
		fgets(s, size + 10, f);
		/* copy the string to the life board */
		for (i = 0; i < size; i++)
		{
			// c=fgetc(f);
			// putchar(c);
			board[i * size + j] = s[i] == 'x';
		}
		// fscanf(f,"\n");
	}
	free(s);
}

/**
 * @brief Main function
 *
 * @param argc number of arguments
 * @param argv arguments
 * @return int return code
 */
int main(int argc, char *argv[])
{
	// MPI variables for rank and number of processes
	int rank, nprocs;

	// Init MPI environment
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	int size, steps;
	FILE *f;
	char source[32];
	int i, j;
	cell_t *board, *local_prev, *local_next, *local_tmp;

	// Variables to store the send counts and displacements for each process (scatterv and gatherv)
	int *sendcounts, *displs;

	// Variables to store the size of the block for each process
	int local_rows, local_cols;

	// Define start and end of the block for each process
	int start, end;

	// Get the arguments
	if (argc < 4)
	{
		printf("Usage: mpirun -np [N] life [<input file>] [size] [steps]\n");
		exit(1);
	}

	size = atoi(argv[2]);
	steps = atoi(argv[3]);

	// Check that the number of processes is at least the number of rows
	if (nprocs > size)
	{
		printf("Error: The number of processes must be at least the number of rows\n");
		exit(1);
	}

#ifdef LOAD_BALANCING

	// Get the number of rows for each process
	int *rows_distribution = load_balancing(nprocs, size, rank, MPI_COMM_WORLD);

	// Compute the size of the block for each process
	local_rows = rows_distribution[rank];
	local_cols = size;

#else

	// Compute the size of the block for each process
	local_rows = size / nprocs;
	local_cols = size;

	// Compute the size of the block of the last process
	if (rank == nprocs - 1)
	{
		local_rows = size - local_rows * (nprocs - 1);
	}

#endif

	// Add the adjacent rows depending on the rank
	if (rank == 0 || rank == nprocs - 1)
	{
		local_rows += ONE_ADJACENT_ROWS;
	}
	else
	{
		local_rows += TWO_ADJACENT_ROWS;
	}

	// Read the input file (only rank 0)
	if (rank == 0)
	{
		// Get the source file from the arguments
		if (argc >= 4)
		{
			strcpy(source, argv[1]);
		}
		else
		{
			printf("Error: No source file specified\n");
			exit(1);
		}

		if ((f = fopen(source, "r")) == NULL)
		{
			printf("Error: Cannot open file %s\n", source);
			exit(1);
		}

		// Allocate memory for the board of the first process
		board = allocate_board(size, size);

		// Read the board from the file
		read_file(f, board, size);

#ifdef PRINT_RESULT

		// Print the board
		printf("--- Initial board ---\n");
		print(board, size);
		printf("---------------------\n\n");

#endif

		fclose(f);

		// Compute the send counts and displacements for each process to scatter(v)	the board
		sendcounts = (int *)malloc(nprocs * sizeof(int));
		displs = (int *)malloc(nprocs * sizeof(int));

#ifdef LOAD_BALANCING

		// Process 0 has the first block
		sendcounts[0] = local_rows * local_cols;
		displs[0] = 0;

		// Middle processes have the block plus 2 rows
		for (i = 1; i < nprocs - 1; i++)
		{
			// The send count is the number of elements of the process block plus 2 rows
			sendcounts[i] = rows_distribution[i] * local_cols + TWO_ADJACENT_ROWS * local_cols;
			// The displacement is the previous displacement plus the number of elements of the previous process block - 2 rows
			displs[i] = displs[i - 1] + sendcounts[i - 1] - TWO_ADJACENT_ROWS * local_cols;
		}

		// The last process has the remaining block plus 1 row
		sendcounts[nprocs - 1] = rows_distribution[nprocs - 1] * local_cols + local_cols;
		displs[nprocs - 1] = displs[nprocs - 2] + sendcounts[nprocs - 2] - TWO_ADJACENT_ROWS * local_cols;

#else

		// Process 0 has the first block
		sendcounts[0] = local_rows * local_cols;
		displs[0] = 0;

		// Middle processes have the block plus 2 rows
		for (i = 1; i < nprocs - 1; i++)
		{
			// The send count is the number of elements of the process block + additional row that is shared with the previous process
			sendcounts[i] = (local_rows + ONE_ADJACENT_ROWS) * local_cols;
			// The displacement is the previous displacement plus the number of elements of the previous process block - 2 rows
			displs[i] = displs[i - 1] + sendcounts[i - 1] - TWO_ADJACENT_ROWS * local_cols;
		}

		// The last process has the remaining block plus 1 row
		int last_block_rows = size - (local_rows - 1) * (nprocs - 1) + ONE_ADJACENT_ROWS;
		sendcounts[nprocs - 1] = last_block_rows * local_cols;
		displs[nprocs - 1] = displs[nprocs - 2] + sendcounts[nprocs - 2] - TWO_ADJACENT_ROWS * local_cols;

#endif
	}

#ifdef DEBUG

	// Print the sendcounts and displacements
	if (rank == 0)
	{
		printf("Sendcounts: ");
		for (i = 0; i < nprocs; i++)
		{
			printf("%d ", sendcounts[i]);
		}
		printf("\n");

		printf("Displacements: ");
		for (i = 0; i < nprocs; i++)
		{
			printf("%d ", displs[i]);
		}
		printf("\n");
	}

#endif

	// Allocate memory for the local board
	local_prev = allocate_board(local_rows, local_cols);
	local_next = allocate_board(local_rows, local_cols);

#ifdef DEBUG

	// Print the size of the local board
	printf("Rank %d: local_rows %d, local_cols %d\n", rank, local_rows, local_cols);

#endif

	// Scatter the board data
	MPI_Scatterv(board, sendcounts, displs, MPI_CHAR, local_prev, local_rows * local_cols, MPI_CHAR, 0, MPI_COMM_WORLD);

#ifdef DEBUG

	// Each process processes its portion of the board (e.g., print)
	for (i = 0; i < local_rows; i++)
	{
		printf("Process %d, Row %d: ", rank, i);
		for (j = 0; j < local_cols; j++)
		{
			printf("%d ", local_prev[i * local_cols + j]);
		}
		printf("\n");
	}
#endif

	// Calculate the start and end of the block for each process
	if (rank == 0)
	{
		start = 0;
		end = local_rows - 1;
	}
	else if (rank == nprocs - 1)
	{
		start = 1;
		end = local_rows;
	}
	else
	{
		start = 1;
		end = local_rows - 1;
	}

#ifdef DEBUG

	// Print the start and end of the block for each process
	printf("Rank %d: start %d, end %d\n", rank, start, end);

#endif

	for (i = 0; i < steps; i++)
	{
		// Apply the rules of the game of life
		play(local_prev, local_next, local_rows, local_cols, start, end);

		// Swap the pointers
		local_tmp = local_next;
		local_next = local_prev;
		local_prev = local_tmp;

		// Exchange the adjacent rows between processes
		if (rank == 0)
		{

			// Send the end row to the next process
			MPI_Send(local_prev + (local_rows - 2) * local_cols, local_cols, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);

			// Receive the last row from the next process
			MPI_Recv(local_prev + (local_rows - 1) * local_cols, local_cols, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		if (rank > 0 && rank < nprocs - 1)
		{
			// Send the start row to the previous process
			MPI_Send(local_prev + local_cols, local_cols, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD);

			// Receive the first row from the previous process
			MPI_Recv(local_prev, local_cols, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			// Send the end row to the next process
			MPI_Send(local_prev + (local_rows - 2) * local_cols, local_cols, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);

			// Receive the last row from the next process
			MPI_Recv(local_prev + (local_rows - 1) * local_cols, local_cols, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		if (rank == nprocs - 1)
		{
			// Send the start row to the previous process
			MPI_Send(local_prev + local_cols, local_cols, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD);

			// Receive the first row from the previous process
			MPI_Recv(local_prev, local_cols, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

#ifdef PRINT
		// Gather the board data
		MPI_Gatherv(local_prev, local_rows * local_cols, MPI_CHAR, board, sendcounts, displs, MPI_CHAR, 0, MPI_COMM_WORLD);

		if (rank == 0)
		{
			printf("--- Iteration %d ---\n", i);
			print(board, size);
			printf("--------------------\n\n");
		}
#endif
	}

#if defined(PRINT_RESULT) || defined(OUTPUT) && !defined(PRINT)

	// Gather the board data
	MPI_Gatherv(local_prev, local_rows * local_cols, MPI_CHAR, board, sendcounts, displs, MPI_CHAR, 0, MPI_COMM_WORLD);

#endif

#if defined(PRINT) || defined(PRINT_RESULT)

	if (rank == 0)
	{
		// Print the board
		printf("--- Final board ---\n");
		print(board, size);
		printf("-------------------\n\n");
	}

#endif

#ifdef OUTPUT

	// Write the board to a file
	if (rank == 0)
	{
		FILE *out_file;
		char dest[32];

		// Get the destination file from the arguments
		if (argc == 5)
		{
			strcpy(dest, argv[4]);
		}
		else
		{
			printf("Error: No destination file specified\n");
			exit(1);
		}

		// Open the file
		if ((out_file = fopen(dest, "w")) == NULL)
		{
			printf("Error: Cannot open file %s\n", dest);
			exit(1);
		}

		// Write the board to the file
		for (j = 0; j < size; j++)
		{
			for (i = 0; i < size; i++)
			{
				fprintf(out_file, "%c", board[i * size + j] ? 'x' : ' ');
			}
			fprintf(out_file, "\n");
		}

		fclose(out_file);
	}

#endif

	// Free memory
	if (rank == 0)
	{
		free_board(board);
		free(sendcounts);
		free(displs);
	}
	free_board(local_prev);
	free_board(local_next);

	// Finalize MPI environment
	MPI_Finalize();

	return 0;
}
