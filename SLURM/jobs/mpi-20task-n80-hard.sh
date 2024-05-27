#!/bin/bash
#SBATCH --job-name=mpi-20task-n80-hard
#SBATCH --output=SLURM/results/mpi-20task-n80-hard.out
#SBATCH --error=SLURM/results/mpi-20task-n80-hard.err
#SBATCH --time=00:10:00
#SBATCH --tasks=20
#SBATCH --nodelist=[n16-80,n16-81,n16-82,n16-83]
#SBATCH --partition=lhpc

srun PARALLEL/bin/life DATA/in/5000.in 5000 100

