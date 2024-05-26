#!/bin/bash
#SBATCH --job-name=sec-n80
#SBATCH --output=sec-n80.out
#SBATCH --error=sec-n80.err
#SBATCH --time=00:10:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --nodelist=n16-80

srun ../BASH/bin/life  DATA/in/1000.in 1000 5 
