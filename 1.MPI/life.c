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

#define TWO_ADJACENT_ROWS 2
#define ONE_ADJACENT_ROWS 1

typedef unsigned char cell_t;

// Descomentar esta línea para ver el tablero en pantalla. Sólo para el fichero life.in
// #define DEBUG 1

/**
 * @brief Allocate memory for the board
 *
 * @param size_rows size of the board
 * @param size_columns size of the board
 * @return cell_t** pointer to the board
 */
cell_t *allocate_board(int size_rows, int size_columns)
{
	cell_t *board = (cell_t *)malloc(size_rows * size_columns * sizeof(cell_t));
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
 * @param size size of the board (size x size)
 * @param i row of the cell
 * @param j column of the cell
 * @return int number of on cells adjacent to the i,j cell
 */
int adjacent_to(cell_t *board, int size, int i, int j)
{
	int k, l, count = 0;

	int sk = (i > 0) ? i - 1 : i;
	int ek = (i + 1 < size) ? i + 1 : i;
	int sl = (j > 0) ? j - 1 : j;
	int el = (j + 1 < size) ? j + 1 : j;

	for (k = sk; k <= ek; k++)
		for (l = sl; l <= el; l++)
			count += board[k * size + l];
	count -= board[i * size + j];

	return count;
}

/**
 * @brief Apply the rules of the game of life to the board
 *
 * @param board board to apply the rules
 * @param newboard new board to store the result
 * @param size size of the board (size x size)
 */
void play(cell_t *board, cell_t *newboard, int size)
{
	int i, j, a;
	/* for each cell, apply the rules of Life */
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
		{
			a = adjacent_to(board, size, i, j);
			/* If a == 2, the cell keeps the same */
			if (a == 2)
				newboard[i * size + j] = board[i * size + j];
			/* If a == 3, then cell lives */
			if (a == 3)
				newboard[i * size + j] = 1;
			/* If a < 2, then cell dies */
			if (a < 2)
				newboard[i * size + j] = 0;
			/* If a > 3, then cell dies */
			if (a > 3)
				newboard[i * size + j] = 0;
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

	int size = 11, steps = 7;
	FILE *f;
	char source[32] = "../DATA/life.in";
	int i, j;
	cell_t *prev, *next, *tmp;
	cell_t *board, *local_board;

	// Variables to store the size of the block for each process
	int local_rows, local_cols;

	size = atoi(argv[2]);
	steps = atoi(argv[3]);

	// Check that the number of processes is at least the number of rows
	if (nprocs > size)
	{
		printf("Error: The number of processes must be at least the number of rows\n");
		exit(1);
	}

	// Compute the size of the block for each process
	local_rows = size / nprocs;
	local_cols = size;

	// Allocate memory for the board of the last process
	if (rank == nprocs - 1)
	{
		local_rows = size - local_rows * (nprocs - 1);
	}

	// Read the input file (only rank 0)
	if (rank == 0)
	{
		// f = stdin;
		// scanf(f,"%d %d", &size, &steps);

		strcpy(source, argv[1]);

		if ((f = fopen(source, "r")) == NULL)
		{
			printf("Error: Cannot open file %s\n", source);
			exit(1);
		}

		// Allocate memory for the board of the first process
		board = allocate_board(size, size);

		// Read the board from the file
		read_file(f, board, size);
		fclose(f);
	}

	// Allocate memory for the local board
	local_board = allocate_board(local_rows, local_cols);

	printf("Rank %d: local_rows %d, local_cols %d\n", rank, local_rows, local_cols);

	// Scatter the board data
	MPI_Scatter(board, local_rows * local_cols, MPI_CHAR, local_board, local_rows * local_cols, MPI_CHAR, 0, MPI_COMM_WORLD);

	// Each process processes its portion of the board (e.g., print)
	for (i = 0; i < local_rows; i++)
	{
		printf("Process %d, Row %d: ", rank, i);
		for (j = 0; j < local_cols; j++)
		{
			printf("%d ", local_board[i * local_cols + j]);
		}
		printf("\n");
	}

	// #ifdef DEBUG
	// 	printf("Initial \n");
	// 	print(prev, size);
	// 	printf("----------\n");
	// #endif

	// 	for (i = 0; i < steps; i++)
	// 	{
	// 		play(prev, next, size);
	// #ifdef DEBUG
	// 		printf("%d ----------\n", i);
	// 		print(next, size);
	// #endif
	// 		tmp = next;
	// 		next = prev;
	// 		prev = tmp;
	// 	}

	// #ifdef DEBUG
	// If DEBUG is defined, print the board, so we need to Gather the board to rank 0
	// 	print(prev, size);
	// #endif

	// Free memory
	if (rank == 0)
	{
		free_board(board);
	}
	free_board(local_board);

	// Finalize MPI environment
	MPI_Finalize();

	return 0;
}
