#!/bin/bash
#SBATCH --job-name=sec-n90
#SBATCH --output=SLURM/results/sec-n90.out
#SBATCH --error=SLURM/results/sec-n90.err
#SBATCH --time=00:10:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --nodelist=n16-90
#SBATCH --partition=lhpc

srun BASE/bin/life  DATA/in/5000.in 5000 100

# Path: SLURM/sec-n100.sh
