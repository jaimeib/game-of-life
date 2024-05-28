#!/bin/bash
#SBATCH --job-name=sec-n80-4000
#SBATCH --output=SLURM/results/sec-n80-4000.out
#SBATCH --error=SLURM/results/sec-n80-4000.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --nodelist=n16-83
#SBATCH --partition=lhpc

prun BASE/bin/life  DATA/in/4000.in 4000 10
