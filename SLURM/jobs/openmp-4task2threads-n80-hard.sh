#!/bin/bash
#SBATCH --job-name=openmp-4task2threads-n80-hard
#SBATCH --output=SLURM/results/openmp-4task2threads-n80-hard.out
#SBATCH --error=SLURM/results/openmp-4task2threads-n80-hard.err
#SBATCH --time=00:10:00
#SBATCH --tasks=4
#SBATCH --nodelist=[n16-80,n16-81,n16-82,n16-83]
#SBATCH --partition=lhpc

export OMP_NUM_THREADS=2
srun PARALLEL/bin/life_openmp DATA/in/5000.in 5000 100
