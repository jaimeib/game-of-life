#!/bin/bash
#SBATCH --job-name=sec-n80
#SBATCH --output=SLURM/results/sec-n80.out
#SBATCH --error=SLURM/results/sec-n80.err
#SBATCH --time=00:05:00
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --nodelist=n16-[80-83]
#SBATCH --partition=lhpc

prun BASE/bin/life  DATA/in/5000.in 5000 100
