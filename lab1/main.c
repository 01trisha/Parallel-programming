#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000
#define EPSILON 0.0000001
#define TAU 0.00001
#define MAX_ITERATION_COUNT 100000

void generate_A(double *A) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; ++j)
      A[i * N + j] = 1;

    A[i * N + i] = 2;
  }
}

void generate_x(double *x) {
  for (int i = 0; i < N; i++)
    x[i] = 0;
}

void generate_b(double *b) {
  for (int i = 0; i < N; i++)
    b[i] = N + 1;
}

double calc_norm_square(const double *vector) {
  double norm_square = 0.0;
  for (int i = 0; i < N; ++i)
    norm_square += vector[i] * vector[i];

  return norm_square;
}

void calc_Axb(const double *A, const double *x, const double *b, double *Axb) {
  for (int i = 0; i < N; ++i) {
    Axb[i] = -b[i];
    for (int j = 0; j < N; ++j)
      Axb[i] += A[i * N + j] * x[j];
  }
}

void calc_next_x(const double *Axb, const double *x, double *next_x) {
  for (int i = 0; i < N; ++i)
    next_x[i] = x[i] - TAU * Axb[i];
}


int main() {
  clock_t start_time, end_time;
  double cpu_time_used;

  double b_norm;
  double accuracy = EPSILON + 1;
  double *A;
  double *x;
  double *b;
  double *Axb;
  double *next_x;

  A = malloc(sizeof(double) * N * N);
  x = malloc(sizeof(double) * N);
  b = malloc(sizeof(double) * N);
  generate_A(A);
  generate_x(x);
  generate_b(b);

  b_norm = sqrt(calc_norm_square(b));

  Axb = malloc(sizeof(double) * N);
  next_x = malloc(sizeof(double) * N);

  start_time = clock();

  int iter_count;
  for (iter_count = 0; accuracy > EPSILON && iter_count < MAX_ITERATION_COUNT; ++iter_count) {
    calc_Axb(A, x, b, Axb);
    calc_next_x(Axb, x, next_x);

    for (int i = 0; i < N; ++i)
      x[i] = next_x[i];

    accuracy = sqrt(calc_norm_square(Axb)) / b_norm;
  }

  end_time = clock();
  cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

  printf("Iteration count: %d\n", iter_count);

  printf("Матрица A:\n");
  for (int i = 0; i < 10; ++i){
      for (int j = 0; j < 10; ++j) {
          printf("%6.2lf ", A[i * N + j]);
      }
      printf("\n");
  }
  printf("\n");

  printf("Вектор B:\n");
  for (int i = 0; i < 10; ++i){
      printf("B[%d] = %lf\n", i, b[i]);
  }
  printf("\n");

  printf("Вектор X:\n");
  for (int i = 0; i < 10; ++i){
      printf("x[%d] = %lf\n", i, x[i]);
  }

  printf("\nNorm: %lf\n", sqrt(calc_norm_square(x)));
  printf("Time: %lf sec\n", cpu_time_used);

  free(x);
  free(b);
  free(A);
  free(Axb);
  free(next_x);

  return 0;
}

