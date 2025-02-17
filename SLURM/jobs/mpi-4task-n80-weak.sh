#!/bin/bash
#SBATCH --job-name=mpi-4task-n80-weak
#SBATCH --output=SLURM/results/mpi-4task-n80-weak.out
#SBATCH --error=SLURM/results/mpi-4task-n80-weak.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=4
#SBATCH --nodelist=n16-[80-83]
#SBATCH --partition=lhpc
#SBATCH --mem=0

prun PARALLEL/bin/life_mpi DATA/in/2000.in 2000 10

