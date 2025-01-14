#!/bin/bash

# Run the jobs
echo "Running the jobs..."

export PATH=$PATH:/opt/ohpc/pub/mpi/openmpi4-gnu9/4.1.1/bin

# Get the list of all the job scripts in the jobs directory
job_scripts=$(ls SLURM/jobs)

# Loop through all the job scripts
for job_script in $job_scripts
do
    # Submit the job script to the SLURM scheduler
    sbatch SLURM/jobs/$job_script
done

echo "All jobs submitted!"

