#!/bin/bash
#SBATCH --job-name=mpi-16task-n80-weak
#SBATCH --output=SLURM/results/mpi-16task-n80-weak.out
#SBATCH --error=SLURM/results/mpi-16task-n80-weak.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=16
#SBATCH --nodelist=n16-[80-83]
#SBATCH --mem=0
#SBATCH --partition=lhpc

prun PARALLEL/bin/life_mpi DATA/in/4000.in 4000 100

