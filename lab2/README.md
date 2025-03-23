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

## время выполнения main
67.404712 sec
## время выполнения omp_parallel:
threads = 1: 67.114254 sec
threads = 2: 33.871753 sec
threads = 4: 19.229057 sec
threads = 8: 9.703501 sec
threads = 16: 8.821988 sec
## время выполнения omp_parallel_for:
threads = 1: 68.392633 sec
threads = 2: 33.878401 sec
threads = 4: 19.211936 sec
threads = 8: 9.737794 sec
threads = 16: 8.764055 sec
## вывод с schedule
DYNAMIC
default chunk
Time: 34.076248 sec
chunk = 1
Time: 34.162942 sec
chunk = 5
Time: 18.576052 sec
chunk = 10
Time: 9.504242 sec
chunk = 20
Time: 9.249238 sec
chunk = 50
Time: 9.487555 sec
chunk = 100
Time: 10.849393 sec