#!/bin/bash
#SBATCH -J trisha 
#SBATCH -p compclass
#SBATCH -e openmp.err
#SBATCH -o dynamicRes2.out
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -t 00:10:00

# Сбрасываем переменные, если они были заданы ранее
unset OMP_NUM_THREADS
unset OMP_SCHEDULE

echo DYNAMIC
echo threads = 4
echo default chunk
export OMP_NUM_THREADS=4 OMP_SCHEDULE="dynamic"; ./omp_parallel_schedule

echo chunk = 1
export OMP_NUM_THREADS=4 OMP_SCHEDULE="dynamic,1"; ./omp_parallel_schedule

echo chunk = 5
export OMP_NUM_THREADS=4 OMP_SCHEDULE="dynamic,5"; ./omp_parallel_schedule

echo chunk = 10
export OMP_NUM_THREADS=4 OMP_SCHEDULE="dynamic,10"; ./omp_parallel_schedule

echo chunk = 20
export OMP_NUM_THREADS=4 OMP_SCHEDULE="dynamic,20"; ./omp_parallel_schedule

echo chunk = 50
export OMP_NUM_THREADS=4 OMP_SCHEDULE="dynamic,50"; ./omp_parallel_schedule

echo chunk = 100
export OMP_NUM_THREADS=4 OMP_SCHEDULE="dynamic,100"; ./omp_parallel_schedule

echo threads = 8
export OMP_NUM_THREADS=8 OMP_SCHEDULE="dynamic"; ./omp_parallel_schedule

echo chunk = 1
export OMP_NUM_THREADS=8 OMP_SCHEDULE="dynamic,1"; ./omp_parallel_schedule

echo chunk = 5
export OMP_NUM_THREADS=8 OMP_SCHEDULE="dynamic,5"; ./omp_parallel_schedule

echo chunk = 10
export OMP_NUM_THREADS=8 OMP_SCHEDULE="dynamic,10"; ./omp_parallel_schedule

echo chunk = 20
export OMP_NUM_THREADS=8 OMP_SCHEDULE="dynamic,20"; ./omp_parallel_schedule

echo chunk = 50
export OMP_NUM_THREADS=8 OMP_SCHEDULE="dynamic,50"; ./omp_parallel_schedule

echo chunk = 100
export OMP_NUM_THREADS=8 OMP_SCHEDULE="dynamic,100"; ./omp_parallel_schedule

echo threads = 16
export OMP_NUM_THREADS=16 OMP_SCHEDULE="dynamic"; ./omp_parallel_schedule

echo chunk = 1
export OMP_NUM_THREADS=16 OMP_SCHEDULE="dynamic,1"; ./omp_parallel_schedule

echo chunk = 5
export OMP_NUM_THREADS=16 OMP_SCHEDULE="dynamic,5"; ./omp_parallel_schedule

echo chunk = 10
export OMP_NUM_THREADS=16 OMP_SCHEDULE="dynamic,10"; ./omp_parallel_schedule

echo chunk = 20
export OMP_NUM_THREADS=16 OMP_SCHEDULE="dynamic,20"; ./omp_parallel_schedule

echo chunk = 50
export OMP_NUM_THREADS=16 OMP_SCHEDULE="dynamic,50"; ./omp_parallel_schedule

echo chunk = 100
export OMP_NUM_THREADS=16 OMP_SCHEDULE="dynamic,100"; ./omp_parallel_schedule
