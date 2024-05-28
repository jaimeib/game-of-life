#!/bin/bash
#SBATCH --job-name=sec-n80-2000
#SBATCH --output=SLURM/results/sec-n80-2000.out
#SBATCH --error=SLURM/results/sec-n80-2000.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --nodelist=n16-81
#SBATCH --partition=lhpc

prun BASE/bin/life  DATA/in/2000.in 2000 10
