#!/bin/bash
#SBATCH --job-name=mpi-4task-n80-hard
#SBATCH --output=SLURM/results/mpi-4task-n80-hard.out
#SBATCH --error=SLURM/results/mpi-4task-n80-hard.err
#SBATCH --time=00:02:00
#SBATCH --tasks=4
#SBATCH --nodelist=n16-[80-83]
#SBATCH --partition=lhpc

srun PARALLEL/bin/life_mpi DATA/in/5000.in 5000 100