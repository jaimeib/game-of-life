#!/bin/bash
#SBATCH --job-name=sec-n80-3000
#SBATCH --output=SLURM/results/sec-n80-3000.out
#SBATCH --error=SLURM/results/sec-n80-3000.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --nodelist=n16-82
#SBATCH --partition=lhpc

prun BASE/bin/life  DATA/in/3000.in 3000 10
