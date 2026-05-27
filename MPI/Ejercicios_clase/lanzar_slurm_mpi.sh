#!/bin/bash

#SBATCH --job-name=PRUEBA
#SBATCH --output=salida.out
#SBATCH --error=error.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --threads-per-core=1
#SBATCH --exclusive
#SBATCH --qos=randall_4c_1t_1min
#SBATCH --mem=1024
#SBATCH --gres=gpu:0
#SBATCH --partition=debug

srun mpirun -np 1 ej
