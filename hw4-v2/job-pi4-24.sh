#!/bin/bash
#SBATCH -J pi4-24
#SBATCH -o pi4-24-%j.out
#SBATCH -N 1
#SBATCH -p RM-shared
#SBATCH --time=00:10:00
module load openmpi/4.1.1-gcc8.3.1
g++ -fopenmp pi4.cpp -o pi4
srun ./pi4 800000 24
