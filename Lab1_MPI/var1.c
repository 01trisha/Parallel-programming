#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 5000
#define EPSILON 0.000001
#define TAU 0.00001
#define MAX_ITERATION_COUNT 100000

void generate_A_chunks(double *A_chunk, int line_count, int line_size,
                       int lineIndex) {
  for (int i = 0; i < line_count; i++) {
    for (int j = 0; j < line_size; ++j) {
      A_chunk[i * line_size + j] = 1;
    }
    A_chunk[i * line_size + lineIndex + i] = 2;
  }
}

void generate_x(double *x, int size) {
  for (int i = 0; i < size; i++) {
    x[i] = 0;
  }
}

void generate_b(double *b, int size) {
  for (int i = 0; i < size; i++) {
    b[i] = N + 1;
  }
}

void set_matrix_part(int *line_counts, int *line_offsets, int size,
                     int process_count) {
  int offset = 0;
  for (int i = 0; i < process_count; ++i) {
    line_counts[i] = size / process_count;

    if (i < size % process_count) {
      ++line_counts[i];
    }
    line_offsets[i] = offset;
    offset += line_counts[i];
  }
}

double calc_norm_square(const double *vector, int size) {
  double norm_square = 0.0;
  for (int i = 0; i < size; ++i) {
    norm_square += vector[i] * vector[i];
  }
  return norm_square;
}

void calc_Axb(const double *A_chunk, const double *x, const double *b,
              double *Axb_chunk, int chunk_size, int chunk_offset) {
  for (int i = 0; i < chunk_size; ++i) {
    Axb_chunk[i] = -b[chunk_offset + i];
    for (int j = 0; j < N; ++j) {
      Axb_chunk[i] += A_chunk[i * N + j] * x[j];
    }
  }
}

void calc_next_x(const double *Axb_chunk, const double *x, double *x_chunk,
                 double tau, int chunk_size, int chunk_offset) {
  for (int i = 0; i < chunk_size; ++i) {
    x_chunk[i] = x[chunk_offset + i] - tau * Axb_chunk[i];
  }
}

int main(int argc, char **argv) {
  int process_rank;
  int process_count;
  int iter_count;
  double b_norm;
  double accuracy = EPSILON + 1;
  double start_time;
  double finish_time;
  int *line_counts;
  int *line_offsets;
  double *A_chunk;
  double *x;
  double *b;
  double *Axb_chunk;
  double *x_chunk;

  MPI_Init(&argc, &argv);

  // общее число процессов
  MPI_Comm_size(MPI_COMM_WORLD, &process_count);
  // номер текущего процесса
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  line_counts = (int *)malloc(sizeof(int) * process_count);
  line_offsets = (int *)malloc(sizeof(int) * process_count);
  set_matrix_part(line_counts, line_offsets, N, process_count);

  A_chunk = (double *)malloc(sizeof(double) * line_counts[process_rank] * N);
  x = (double *)malloc(sizeof(double) * N);
  b = (double *)malloc(sizeof(double) * N);
  generate_A_chunks(A_chunk, line_counts[process_rank], N,
                    line_offsets[process_rank]);
  generate_x(x, N);
  generate_b(b, N);

  if (process_rank == 0) {
    b_norm = sqrt(calc_norm_square(b, N));
  }

  Axb_chunk = (double *)malloc(sizeof(double) * line_counts[process_rank]);
  x_chunk = (double *)malloc(sizeof(double) * line_counts[process_rank]);

  start_time = MPI_Wtime();

  for (iter_count = 0; accuracy > EPSILON && iter_count < MAX_ITERATION_COUNT;
       ++iter_count) {
    calc_Axb(A_chunk, x, b, Axb_chunk, line_counts[process_rank],
             line_offsets[process_rank]);

    calc_next_x(Axb_chunk, x, x_chunk, TAU, line_counts[process_rank],
                line_offsets[process_rank]);
    // каждый процесс отдает кусок данных, собираем x
    MPI_Allgatherv(x_chunk, line_counts[process_rank], MPI_DOUBLE, x,
                   line_counts, line_offsets, MPI_DOUBLE, MPI_COMM_WORLD);

    double Axb_chunk_norm_square =
        calc_norm_square(Axb_chunk, line_counts[process_rank]);

    // суммируем точность
    MPI_Reduce(&Axb_chunk_norm_square, &accuracy, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD);
    if (process_rank == 0) {
      accuracy = sqrt(accuracy) / b_norm;
    }
    // отправляем точность всем процессам
    MPI_Bcast(&accuracy, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  finish_time = MPI_Wtime();
  if (process_rank == 0) {
    if (iter_count == MAX_ITERATION_COUNT) {
      printf("Too many iterations\n");
    } else {
      printf("Norm: %lf\n", sqrt(calc_norm_square(x, N)));
      printf("Time: %lf sec\n", finish_time - start_time);
      printf("Iteration count: %d\n", iter_count);

      printf("Матрица A:\n");
      for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
          printf("%6.2lf ", A_chunk[i * N + j]);
        }
        printf("\n");
      }
      printf("\n");

      printf("Вектор B:\n");
      for (int i = 0; i < 10; ++i) {
        printf("B[%d] = %lf\n", i, b[i]);
      }
      printf("\n");

      printf("Вектор X:\n");
      for (int i = 0; i < 10; ++i) {
        printf("x[%d] = %lf\n", i, x[i]);
      }
    }
  }

  free(line_counts);
  free(line_offsets);
  free(x);
  free(b);
  free(A_chunk);
  free(Axb_chunk);
  free(x_chunk);

  MPI_Finalize();

  return 0;
}
