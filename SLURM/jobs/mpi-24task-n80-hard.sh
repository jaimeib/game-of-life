#!/bin/bash
#SBATCH --job-name=mpi-24task-n80-hard
#SBATCH --output=SLURM/results/mpi-24task-n80-hard.out
#SBATCH --error=SLURM/results/mpi-24task-n80-hard.err
#SBATCH --time=00:10:00
#SBATCH --tasks=24
#SBATCH --nodelist=n16-[80-83]
#SBATCH --partition=lhpc

prun PARALLEL/bin/life_mpi DATA/in/5000.in 5000 100

