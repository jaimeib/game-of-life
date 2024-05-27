#!/bin/bash
#SBATCH --job-name=mpi-w-lb-40task
#SBATCH --output=SLURM/results/mpi-w-lb-40task.out
#SBATCH --error=SLURM/results/mpi-w-lb-40task.err
#SBATCH --time=00:02:00
#SBATCH --tasks=36
#SBATCH --partition=lhpc

prun PARALLEL/bin/life_load_balancing_mpi DATA/in/5000.in 5000 100

