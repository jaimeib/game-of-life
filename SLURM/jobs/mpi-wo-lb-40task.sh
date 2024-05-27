#!/bin/bash
#SBATCH --job-name=mpi-wo-lb-40task
#SBATCH --output=SLURM/results/mpi-wo-lb-40task.out
#SBATCH --error=SLURM/results/mpi-wo-lb-40task.err
#SBATCH --time=00:02:00
#SBATCH --tasks=36
#SBATCH --partition=lhpc

srun PARALLEL/bin/life_mpi DATA/in/5000.in 5000 100

