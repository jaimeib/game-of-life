#!/bin/bash
#SBATCH --job-name=openmp-w-lb-7task
#SBATCH --output=SLURM/results/openmp-w-lb-7task.out
#SBATCH --error=SLURM/results/openmp-w-lb-7task.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=7
#SBATCH --nodes=7
#SBATCH --nodelist=n16-[80-83,90,92-93]
#SBATCH --mem=0
#SBATCH --exclusive
#SBATCH --partition=lhpc

prun PARALLEL/bin/life_load_balancing_openmp DATA/in/5000.in 5000 100

