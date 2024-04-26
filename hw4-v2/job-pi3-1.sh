#!/bin/bash
#SBATCH -J pi3-1
#SBATCH -o pi3-1-%j.out
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -p RM-shared
#SBATCH --time=00:10:00
module load openmpi/4.1.1-gcc8.3.1
mpic++ pi3.cpp -o pi3
mpirun -np 1 --bind-to core:overload-allowed ./pi3 800000
