#!/bin/bash
#SBATCH -J pi2-1
#SBATCH -o pi2-1-%j.out
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -p RM-shared
#SBATCH --time=00:10:00
module load openmpi/4.1.1-gcc8.3.1
mpic++ pi2.cpp -o pi2
mpirun -np 1 --bind-to core:overload-allowed ./pi2 800000
