#!/bin/bash
#SBATCH --job-name=mpi-wo-lb-40task
#SBATCH --output=SLURM/results/mpi-wo-lb-40task.out
#SBATCH --error=SLURM/results/mpi-wo-lb-40task.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=36
#SBATCH --nodes=7
#SBATCH --nodelist=n16-[80-83,90,92-93]
#SBATCH --partition=lhpc

prun PARALLEL/bin/life_mpi DATA/in/5000.in 5000 100

