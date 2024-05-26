/************************************************************/
/* Load Balancing: Life header file                         */
/************************************************************/

/* Define cluster nodes (Dictionary mapping hostnames to frequencies) */
typedef struct
{
    char *hostname[100];
    int niceness;
} ClusterNode;

/* Define cluster (Array of cluster nodes) */
typedef struct
{
    ClusterNode *nodes;
    int size;
} Cluster;

/* Define cluster configuration */
Cluster cluster = {
    .nodes = (ClusterNode[]){
        {.hostname = "n16-80", .niceness = 2},
        {.hostname = "n16-81", .niceness = 2},
        {.hostname = "n16-82", .niceness = 2},
        {.hostname = "n16-83", .niceness = 2},
        {.hostname = "n16-90", .niceness = 1},
        {.hostname = "n16-91", .niceness = 1},
        {.hostname = "n16-92", .niceness = 1},
        {.hostname = "n16-93", .niceness = 1},
    },
    .size = 8,
};

/************************************************************/

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

void get_hostname(char *hostname, int size)
{
    FILE *file = fopen("/proc/sys/kernel/hostname", "r");
    if (file == NULL)
    {
        perror("fopen");
        exit(1);
    }

    if (fgets(hostname, size, file) == NULL)
    {
        perror("fgets");
        exit(1);
    }

    fclose(file);
}

/************************************************************/

/**
 * @brief Static load balancing function
 * @param nprocs number of processes in the cluster
 * @param size number of rows in the grid
 * @param rank rank of the current process
 * @param comm MPI communicator
 * @return an array with the number of rows assigned to each process
 */
int *load_balancing(int nprocs, int size, int rank, MPI_Comm comm)
{

    // Check if the number of processes is the cluster size
    if (nprocs != cluster.size)
    {
        fprintf(stderr, "Number of processes must be equal to the cluster size\n");
        exit(1);
    }

    // Allocate memory for the niceness of each process
    int *niceness_by_rank = (int *)malloc(nprocs * sizeof(int));

    // Get the hostname of the current process
    char hostname[100];
    get_hostname(hostname, 100);

    // Find the current process in the cluster an get its niceness
    niceness_by_rank[rank] = 0;

    for (int i = 0; i < cluster.size; i++)
    {
        if (strcmp(*cluster.nodes[i].hostname, hostname) != 0)
        {
            niceness_by_rank[rank] = cluster.nodes[i].niceness;
            break;
        }
    }

#ifdef DEBUG
    printf("Rank %d: Niceness %d\n", rank, niceness_by_rank[rank]);
#endif

    // Share the niceness of the current process with the other processes
    MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, niceness_by_rank, 1, MPI_INT, comm);

    // Compute the total niceness
    int total_niceness = 0;

    for (int i = 0; i < cluster.size; i++)
    {
        total_niceness += niceness_by_rank[i];
    }

    // Check if the total niceness is > 0
    if (total_niceness <= 0)
    {
        fprintf(stderr, "Total niceness must be greater than 0\n");
        exit(1);
    }

    // Compute the number of rows assigned to each process
    int *rows = (int *)malloc(nprocs * sizeof(int));

    for (int i = 0; i < nprocs; i++)
    {
        rows[i] = 0;
    }

    // Assign more rows to processes with higher niceness
    for (int i = 0; i < nprocs; i++)
    {
        rows[i] = (size / total_niceness) * niceness_by_rank[i];
    }

    // Assign the remaining rows to the first processes
    for (int i = 0; i < size % total_niceness; i++)
    {
        rows[i]++;
    }

    return rows;
}
