#!/bin/bash
#SBATCH --job-name=mpi-1task-n80-hard
#SBATCH --output=SLURM/results/mpi-1task-n80-hard.out
#SBATCH --error=SLURM/results/mpi-1task-n80-hard.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --nodelist=n16-83
#SBATCH --partition=lhpc
#SBATCH --mem=0

prun PARALLEL/bin/life_mpi DATA/in/5000.in 5000 100

