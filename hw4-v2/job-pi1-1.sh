#!/bin/bash
#SBATCH -J pi1-1
#SBATCH -o pi1-1-%j.out
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -p RM-shared
#SBATCH --time=00:10:00
module load openmpi/4.1.1-gcc8.3.1
mpic++ pi1.cpp -o pi1
mpirun -np 1 --bind-to core:overload-allowed ./pi1 800000
