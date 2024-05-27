#!/bin/bash
#SBATCH --job-name=openmp-w-lb-40task
#SBATCH --output=SLURM/results/openmp-w-lb-40task.out
#SBATCH --error=SLURM/results/openmp-w-lb-40task.err
#SBATCH --time=00:10:00
#SBATCH --tasks=7
#SBATCH --exclusive
#SBATCH --partition=lhpc

prun PARALLEL/bin/life_load_balancing_openmp DATA/in/5000.in 5000 100

