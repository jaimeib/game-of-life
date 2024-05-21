/**
 * @file generator.c
 * @author Jaime Iglesias Blanco
 * @brief Conway Game of Life square board generator
 * @version 0.1
 * @date 2024-05-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    if (argc != 5)
    {
        printf("Usage: %s <size> <seed> <dispersion> <output>\n", argv[0]);
        exit(1);
    }

    int size = atoi(argv[1]);
    int seed = atoi(argv[2]);
    int dispersion = atoi(argv[3]);
    char *output = argv[4];

    FILE *f = fopen(output, "w");
    if (f == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    srand(seed);

    fprintf(f, "%d\n", size);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // Randomly generate 'x' or ' ' characters
            fprintf(f, "%c", rand() % dispersion == 0 ? 'x' : ' ');
        }
        fprintf(f, "\n");
    }

    fclose(f);

    exit(0);
}
