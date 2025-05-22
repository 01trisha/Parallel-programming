mpicc -pthread main.c -o main -lm
mpirun -np N ./main
