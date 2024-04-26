#!/bin/bash
#SBATCH -J pi2-24
#SBATCH -o pi2-24-%j.out
#SBATCH -N 1
#SBATCH -n 24
#SBATCH -p RM-shared
#SBATCH --time=00:10:00
module load openmpi/4.1.1-gcc8.3.1
mpic++ pi2.cpp -o pi2
mpirun -np 24 --bind-to core:overload-allowed ./pi2 800000
