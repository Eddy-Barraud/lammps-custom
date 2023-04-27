#!/bin/bash

#SBATCH -N 1
#SBATCH -n 36
#SBATCH -J Newton
#SBATCH --exclusive
#SBATCH -e job.err
#SBATCH -o job.out
#SBATCH --wckey xfq39001
#SBATCH --time=3-2:0:0
#SBATCH -p gpgpu
#SBATCH --gres=gpu:1

module purge
module load intel/2021a zstd/1.4.9 CUDA/11.3.1

make clean-all
make clean-machine
make no-OPENMP
make no-INTEL
make no-installed

#make lib-gpu args="-m mpi -a sm_60 -p double -b"
make lib-gpu args="-m mpi -a sm_60 -p mixed -b"
make yes-KSPACE
make yes-MANYBODY
make yes-MC
make yes-MISC
make yes-MOLECULE
make yes-RIGID
make yes-CLASS2
make yes-EXTRA-MOLECULE
make yes-EXTRA-PAIR
make yes-DPD-BASIC
make yes-DPD-MESO
make yes-COMPRESS
make yes-GPU
make yes-MPIIO
#make yes-OPENMP
make yes-INTEL
# Go
#make -j 8 icc_openmpi
make -j 36 intel_cpu_intelmpi
