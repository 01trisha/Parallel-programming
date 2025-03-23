## Вариант задачи:
Метод простой итерации

## Запуск
### omp_parallel_for.c:
    на сервере: gcc -fopenmp omp_parallel_for.c -o omp_parallel_for 
    на мак: clang -fopenmp=libomp omp_parallel_for.c -o omp_parallel_for
    export OMP_NUM_THREADS=N
    ./omp_parallel_for

### omp_parallel.c:
    на сервере: gcc -fopenmp -o omp_parallel omp_parallel.c
    на мак: clang -fopenmp -o omp_parallel omp_parallel.c
    export OMP_NUM_THREADS=N
    ./omp_parallel

