#!/bin/bash
#SBATCH --job-name=mpi-2task-n80-hard
#SBATCH --output=SLURM/results/mpi-2task-n80-hard.out
#SBATCH --error=SLURM/results/mpi-2task-n80-hard.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=2
#SBATCH --nodelist=n16-[80-83]
#SBATCH --partition=lhpc
#SBATCH --mem=0

prun PARALLEL/bin/life_mpi DATA/in/5000.in 5000 10

