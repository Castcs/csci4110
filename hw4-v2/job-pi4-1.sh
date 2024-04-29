#!/bin/bash
#SBATCH -J pi4-1
#SBATCH -o pi4-1-%j.out
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -p RM-shared
#SBATCH --time=00:10:00
module load openmpi/4.1.1-gcc8.3.1
g++ -fopenmp pi4.cpp -o pi4
srun ./pi4 800000 1
