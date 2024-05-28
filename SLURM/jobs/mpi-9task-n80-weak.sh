#!/bin/bash
#SBATCH --job-name=mpi-9task-n80-weak
#SBATCH --output=SLURM/results/mpi-9task-n80-weak.out
#SBATCH --error=SLURM/results/mpi-9task-n80-weak.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=9
#SBATCH --nodelist=n16-[80-83]
#SBATCH --partition=lhpc
#SBATCH --mem=0

prun PARALLEL/bin/life_mpi DATA/in/3000.in 3000 10

