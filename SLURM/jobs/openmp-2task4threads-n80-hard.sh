#!/bin/bash
#SBATCH --job-name=openmp-2task4threads-n80-hard
#SBATCH --output=SLURM/results/openmp-2task4threads-n80-hard.out
#SBATCH --error=SLURM/results/openmp-2task4threads-n80-hard.err
#SBATCH --time=00:10:00
#SBATCH --tasks=2
#SBATCH --nodelist=n16-[80-83]
#SBATCH --partition=lhpc

export OMP_NUM_THREADS=4
srun PARALLEL/bin/life_openmp DATA/in/5000.in 5000 100

