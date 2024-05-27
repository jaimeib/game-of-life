#!/bin/bash
#SBATCH --job-name=openmp-wo-lb-40task
#SBATCH --output=SLURM/results/openmp-wo-lb-40task.out
#SBATCH --error=SLURM/results/openmp-wo-lb-40task.err
#SBATCH --time=00:10:00
#SBATCH --tasks=7
#SBATCH --nodelist=n16-[80-83],n16-[90-93]
#SBATCH --exclusive
#SBATCH --partition=lhpc

prun PARALLEL/bin/life_openmp DATA/in/5000.in 5000 100

