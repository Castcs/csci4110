#!/bin/bash
#SBATCH -J pi4-48
#SBATCH -o pi4-48-%j.out
#SBATCH -N 1
#SBATCH -n 48
#SBATCH -p RM-shared
#SBATCH --time=00:10:00
module load openmpi/4.1.1-gcc8.3.1
mpic++ pi4.cpp -o pi4
mpirun -np 48 --bind-to core:overload-allowed ./pi4 800000
