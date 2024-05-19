/***********************************************************************/
/* Nombre: Jaime Iglesias Blanco                                       */
/* Práctica:                                                           */
/* Fecha: 					                                           */
/*                                                                     */
/* Usage: ./life [<input file>] [dimension tablero] [numero juegadas]  */
/*                                                                     */
/***********************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>

typedef unsigned char cell_t;

// Descomentar esta línea para ver el tablero en pantalla. Sólo para el fichero life.in
// #define DEBUG 1

/**
 * @brief Allocate memory for the board
 *
 * @param size size of the board (size x size)
 * @return cell_t** pointer to the board
 */
cell_t **allocate_board(int size)
{
	cell_t **board = (cell_t **)malloc(sizeof(cell_t *) * size);
	int i;
	for (i = 0; i < size; i++)
		board[i] = (cell_t *)malloc(sizeof(cell_t) * size);
	return board;
}

/**
 * @brief Free the memory allocated for the board
 *
 * @param board board to free
 * @param size size of the board (size x size)
 */
void free_board(cell_t **board, int size)
{
	int i;
	for (i = 0; i < size; i++)
		free(board[i]);
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
int adjacent_to(cell_t **board, int size, int i, int j)
{
	int k, l, count = 0;

	int sk = (i > 0) ? i - 1 : i;
	int ek = (i + 1 < size) ? i + 1 : i;
	int sl = (j > 0) ? j - 1 : j;
	int el = (j + 1 < size) ? j + 1 : j;

	for (k = sk; k <= ek; k++)
		for (l = sl; l <= el; l++)
			count += board[k][l];
	count -= board[i][j];

	return count;
}

/**
 * @brief Apply the rules of the game of life to the board
 *
 * @param board board to apply the rules
 * @param newboard new board to store the result
 * @param size size of the board (size x size)
 */
void play(cell_t **board, cell_t **newboard, int size)
{
	int i, j, a;
	/* for each cell, apply the rules of Life */
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
		{
			a = adjacent_to(board, size, i, j);
			/* If a == 2, the cell keeps the same */
			if (a == 2)
				newboard[i][j] = board[i][j];
			/* If a == 3, then cell lives */
			if (a == 3)
				newboard[i][j] = 1;
			/* If a < 2, then cell dies */
			if (a < 2)
				newboard[i][j] = 0;
			/* If a > 3, then cell dies */
			if (a > 3)
				newboard[i][j] = 0;
		}
}

/**
 * @brief Print the board in the screen
 *
 * @param board board to print
 * @param size size of the board (size x size)
 */
void print(cell_t **board, int size)
{
	int i, j;
	/* for each row */
	for (j = 0; j < size; j++)
	{
		/* print each column position... */
		for (i = 0; i < size; i++)
			printf("%c", board[i][j] ? 'x' : ' ');
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
void read_file(FILE *f, cell_t **board, int size)
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
			board[i][j] = s[i] == 'x';
		}
		// fscanf(f,"\n");
	}
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

	int size = 11, steps = 7;
	FILE *f;
	char source[32] = "../DATA/life.in";
	int i, j;

	//      f = stdin;
	//	fscanf(f,"%d %d", &size, &steps);

	strcpy(source, argv[1]);
	size = atoi(argv[2]);
	steps = atoi(argv[3]);

	if ((f = fopen(source, "r")) == NULL)
	{
		printf("Error: Cannot open file %s\n", source);
		exit(1);
	}

	cell_t **prev = allocate_board(size);
	read_file(f, prev, size);
	fclose(f);
	cell_t **next = allocate_board(size);
	cell_t **tmp;

#ifdef PRINT_RESULT

	// Print the board
	printf("--- Initial board ---\n");
	print(prev, size);
	printf("---------------------\n\n");

#endif

	for (i = 0; i < steps; i++)
	{
		play(prev, next, size);

		tmp = next;
		next = prev;
		prev = tmp;

#ifdef PRINT
		// Print the board
		printf("--- Iteration %d ---\n", i);
		print(prev, size);
		printf("--------------------\n\n");
#endif
	}

#ifdef PRINT_RESULT
	// Print the board
	printf("--- Final board ---\n");
	print(prev, size);
	printf("-------------------\n\n");
#endif

#ifdef OUTPUT

	// Save the board to a file
	FILE *out_file;
	char dest[32];

	// Get the destination file from the arguments
	if (argc > 4)
	{
		strcpy(dest, argv[4]);
	}
	else
	{
		strcpy(dest, "../DATA/life.out");
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
			fprintf(out_file, "%c", prev[i][j] ? 'x' : ' ');
		}
		fprintf(out_file, "\n");
	}

	fclose(out_file);

#endif

	free_board(prev, size);
	free_board(next, size);
}
