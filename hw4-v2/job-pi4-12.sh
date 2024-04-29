#!/bin/bash
#SBATCH -J pi4-12
#SBATCH -o pi4-12-%j.out
#SBATCH -N 1
#SBATCH -p RM-shared
#SBATCH --time=00:10:00
module load openmpi/4.1.1-gcc8.3.1
g++ -fopenmp pi4.cpp -o pi4
srun ./pi4 800000 12
