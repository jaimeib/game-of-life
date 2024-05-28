#!/bin/bash
#SBATCH --job-name=openmp-4task4threads-n80-hard
#SBATCH --output=SLURM/results/openmp-4task4threads-n80-hard.out
#SBATCH --error=SLURM/results/openmp-4task4threads-n80-hard.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --nodes=4
#SBATCH --nodelist=n16-[80-83]
#SBATCH --partition=lhpc
#SBATCH --mem=0

export OMP_NUM_THREADS=4
prun PARALLEL/bin/life_openmp DATA/in/5000.in 5000 100
