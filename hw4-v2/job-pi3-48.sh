#!/bin/bash
#SBATCH -J pi3-48
#SBATCH -o pi3-48-%j.out
#SBATCH -N 1
#SBATCH -n 48
#SBATCH -p RM-shared
#SBATCH --time=00:10:00
module load openmpi/4.1.1-gcc8.3.1
mpic++ pi3.cpp -o pi3
mpirun -np 48 --bind-to core:overload-allowed ./pi3 800000
