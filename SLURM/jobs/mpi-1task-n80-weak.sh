#!/bin/bash
#SBATCH --job-name=mpi-1task-n80-weak
#SBATCH --output=SLURM/results/mpi-1task-n80-weak.out
#SBATCH --error=SLURM/results/mpi-1task-n80-weak.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=1
#SBATCH --nodelist=n16-[80-83]
#SBATCH --partition=lhpc

prun PARALLEL/bin/life_mpi DATA/in/1000.in 1000 100

