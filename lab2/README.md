## Вариант задачи:
Метод простой итерации

## Запуск
### main:
    gcc main.c -o main -lm
### omp_parallel_for.c:
    на сервере: gcc -fopenmp omp_parallel_for.c -o omp_parallel_for -lm 
    на мак: clang -fopenmp=libomp omp_parallel_for.c -o omp_parallel_for
    export OMP_NUM_THREADS=N
    ./omp_parallel_for

### omp_parallel.c:
    на сервере: gcc -fopenmp -o omp_parallel omp_parallel.c -lm
    на мак: clang -fopenmp -o omp_parallel omp_parallel.c
    export OMP_NUM_THREADS=N
    ./omp_parallel

### omp_parallel_schedule.c:
    chmod +x run_schedule.sh
   на сервере: gcc -fopenmp -o omp_parallel_schedule omp_parallel_schedule.c -lm
   на мак: clang -fopenmp -o omp_parallel_schedule omp_parallel_schedule.c  
   ./run_schedule.sh

   
При условиях нашей матрицы A у нас определяется максимальное TAU по формуле 2/(N+1)
поэтому была для большого N и при этом нормального вектора X использую TAU = 0.0001

в последующих использую нормальное TAU = 0.01 для значимого времени выполнения
