#!/bin/bash
#SBATCH --job-name=mpi-9task-n80-weak
#SBATCH --output=SLURM/results/mpi-9task-n80-weak.out
#SBATCH --error=SLURM/results/mpi-9task-n80-weak.err
#SBATCH --time=00:10:00
#SBATCH --tasks=9
#SBATCH --nodelist=n16-[80-83]
#SBATCH --partition=lhpc

prun PARALLEL/bin/life_mpi DATA/in/3000.in 3000 100

