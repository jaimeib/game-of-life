#!/bin/bash
#SBATCH --job-name=mpi-wo-lb-28task
#SBATCH --output=SLURM/results/mpi-wo-lb-28task.out
#SBATCH --error=SLURM/results/mpi-wo-lb-28task.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=28
#SBATCH --nodes=7
#SBATCH --nodelist=n16-[80-83,90,92-93]
#SBATCH --mem=0
#SBATCH --partition=lhpc

prun PARALLEL/bin/life_mpi DATA/in/5000.in 5000 100

