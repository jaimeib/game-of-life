#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>

#include <mpi.h>

#include "test.h"

int main(int argc, char *argv[])
{
    // MPI variables for rank and number of processes
    int rank, nprocs;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int *board, *localboard;

    // Board size
    int N = 9;
    int iters = 3;

    int local_rows, local_cols;
    int *sendcounts, *displs;
    int start, end;

    MPI_Status status;

    // Get the number of rows for each process
    int *rows_distribution = load_balancing(nprocs, N, rank, MPI_COMM_WORLD);

    printf("Rank %d: rows %d\n", rank, rows_distribution[rank]);

    // Number of rows and columns per process
    local_rows = rows_distribution[rank];
    local_cols = N;

    // Add the adjacent rows depending on the rank
    if (rank == 0 || rank == nprocs - 1)
    {
        local_rows = local_rows + 1;
    }
    else
    {
        local_rows = local_rows + 2;
    }

    // Allocate memory for the local board
    if (rank == 0)
    {
        board = (int *)malloc(N * N * sizeof(int));

        // Initialize the board each line 1, 2, 3, 4, 5, 6, 7, 8, 9
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                board[i * N + j] = i + 1;
            }
        }

        // Print the board
        printf("BOARD\n");
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%d ", board[i * N + j]);
            }
            printf("\n");
        }

        printf("\n");

        // Compute the send counts and displacements for each process to scatter(v)	the board
        sendcounts = (int *)malloc(nprocs * sizeof(int));
        displs = (int *)malloc(nprocs * sizeof(int));

        // Process 0 has the first block
        sendcounts[0] = local_rows * local_cols;
        displs[0] = 0;

        // Middle processes have the block plus 2 rows
        for (int i = 1; i < nprocs - 1; i++)
        {
            // The send count is the number of elements of the process block plus 2 rows
            sendcounts[i] = rows_distribution[i] * local_cols + 2 * local_cols;
            // The displacement is the previous displacement plus the number of elements of the previous process block - 2 rows
            displs[i] = displs[i - 1] + sendcounts[i - 1] - 2 * local_cols;
        }

        // The last process has the remaining block plus 1 row
        sendcounts[nprocs - 1] = rows_distribution[nprocs - 1] * local_cols + local_cols;
        displs[nprocs - 1] = displs[nprocs - 2] + sendcounts[nprocs - 2] - 2 * local_cols;

        printf("Sendcounts: ");
        for (int i = 0; i < nprocs; i++)
        {
            printf("%d ", sendcounts[i]);
        }
        printf("\n");

        printf("Displacements: ");
        for (int i = 0; i < nprocs; i++)
        {
            printf("%d ", displs[i]);
        }
        printf("\n");
    }

    // Allocate memory for the local board
    localboard = (int *)malloc(local_rows * local_cols * sizeof(int));

    // Scatter the board
    MPI_Scatterv(board, sendcounts, displs, MPI_INT, localboard, local_rows * local_cols, MPI_INT, 0, MPI_COMM_WORLD);

    // Print the local board
    printf("BASE BOARD Rank %d\n", rank);
    for (int i = 0; i < local_rows; i++)
    {
        for (int j = 0; j < local_cols; j++)
        {
            printf("%d ", localboard[i * local_cols + j]);
        }
        printf("\n");
    }

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

    // Print the start and end of the block for each process
    printf("Rank %d: start %d, end %d\n", rank, start, end);

    for (int a = 0; a < iters; a++)
    {
        // Update the local board (*10 each element)
        for (int i = start; i < end; i++)
        {
            for (int j = 0; j < local_cols; j++)
            {
                localboard[i * local_cols + j] = localboard[i * local_cols + j] * 10;
            }
        }

        // Print the updated local board
        printf("UPDATED BOARD Rank %d\n", rank);
        for (int i = 0; i < local_rows; i++)
        {
            for (int j = 0; j < local_cols; j++)
            {
                printf("%d ", localboard[i * local_cols + j]);
            }
            printf("\n");
        }

        // Send the adjacent rows to the adjacent processes

        if (rank == 0)
        {
            // Send the end row to the next process
            MPI_Send(localboard + (local_rows - 2) * local_cols, local_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

            // Receive the last row from the next process
            MPI_Recv(localboard + (local_rows - 1) * local_cols, local_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (rank > 0 && rank < nprocs - 1)
        {

            // Send the start row to the previous process
            MPI_Send(localboard + local_cols, local_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

            // Receive the first row from the previous process
            MPI_Recv(localboard, local_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Send the end row to the next process
            MPI_Send(localboard + (local_rows - 2) * local_cols, local_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

            // Receive the last row from the next process
            MPI_Recv(localboard + (local_rows - 1) * local_cols, local_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (rank == nprocs - 1)
        {
            // Send the start row to the previous process
            MPI_Send(localboard + local_cols, local_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

            // Receive the first row from the previous process
            MPI_Recv(localboard, local_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    // Print the new local board
    printf("NEW BOARD Rank %d\n", rank);
    for (int i = 0; i < local_rows; i++)
    {
        for (int j = 0; j < local_cols; j++)
        {
            printf("%d ", localboard[i * local_cols + j]);
        }
        printf("\n");
    }

    // Gather the local boards into the global board
    MPI_Gatherv(localboard, local_rows * local_cols, MPI_INT, board, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    // Print the gathered board
    if (rank == 0)
    {
        printf("GATHERED BOARD\n");
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%d ", board[i * N + j]);
            }
            printf("\n");
        }
    }

    // Free memory
    if (rank == 0)
    {
        free(board);
        free(sendcounts);
        free(displs);
    }

    free(localboard);

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
