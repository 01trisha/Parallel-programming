#include <math.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DIMS_COUNT 2
#define X 0
#define Y 1

void InitDims(int dims[DIMS_COUNT], int countProc, int argc, char **argv) {
  if (argc < 3)
    MPI_Dims_create(countProc, DIMS_COUNT, dims);
  else {
    dims[X] = atoi(argv[1]);
    dims[Y] = atoi(argv[2]);

    if (dims[X] * dims[Y] != countProc)
      exit(EXIT_FAILURE);
  }
}

// Initialize communicators
void InitCommunicators(const int dims[DIMS_COUNT], MPI_Comm *commGrid,
                       MPI_Comm *commRows, MPI_Comm *commColumns) {
  int reorder = 1;
  int periods[DIMS_COUNT] = {};
  int subDims[DIMS_COUNT] = {};

  MPI_Cart_create(MPI_COMM_WORLD, DIMS_COUNT, dims, periods, reorder, commGrid);

  subDims[X] = false;
  subDims[Y] = true;
  MPI_Cart_sub(*commGrid, subDims, commRows);

  subDims[X] = true;
  subDims[Y] = false;
  MPI_Cart_sub(*commGrid, subDims, commColumns);
}

// Generate matrix that has same numbers on rows or columns
void GenerateMatrix(double *matrix, int column, int leadingRow,
                    int leadingColumn, bool onRows) {
  for (int i = 0; i < leadingRow; ++i)
    for (int j = 0; j < leadingColumn; ++j)
      matrix[i * column + j] = onRows ? i : j;
}

// Split matrix A into row blocks between processes
void SplitA(const double *A, double *A_block, int A_blockSize, int n2,
            int yCoords, MPI_Comm commRows, MPI_Comm commColumns) {
  if (yCoords == 0)

  {
    MPI_Scatter(A, A_blockSize * n2, MPI_DOUBLE, A_block, A_blockSize * n2,
                MPI_DOUBLE, 0, commColumns);
  }

  MPI_Bcast(A_block, A_blockSize * n2, MPI_DOUBLE, 0, commRows);
}

// Split matrix B into column blocks between processes
void SplitB(const double *B, double *B_block, int B_blockSize, int n2,
            int aligned_n3, int xCoords, MPI_Comm commRows,
            MPI_Comm commColumns) {
  if (xCoords == 0) {
    MPI_Datatype columnNotResized_t;
    MPI_Datatype columnResized_t;

    MPI_Type_vector(n2, B_blockSize, aligned_n3, MPI_DOUBLE,
                    &columnNotResized_t);
    MPI_Type_commit(&columnNotResized_t);

    MPI_Type_create_resized(columnNotResized_t, 0, B_blockSize * sizeof(double),
                            &columnResized_t);
    MPI_Type_commit(&columnResized_t);

    MPI_Scatter(B, 1, columnResized_t, B_block, B_blockSize * n2, MPI_DOUBLE, 0,
                commRows);

    MPI_Type_free(&columnNotResized_t);
    MPI_Type_free(&columnResized_t);
  }

  MPI_Bcast(B_block, B_blockSize * n2, MPI_DOUBLE, 0, commColumns);
}

// Multiply row block of matrix A and column block of matrix B
void Multi(const double *A_block, const double *B_block, double *C_block,
           int A_blockSize, int B_blockSize, int n2) {
  for (int i = 0; i < A_blockSize; ++i)
    for (int j = 0; j < B_blockSize; ++j)
      C_block[i * B_blockSize + j] = 0;

  for (int i = 0; i < A_blockSize; ++i)
    for (int j = 0; j < n2; ++j)
      for (int k = 0; k < B_blockSize; ++k)
        C_block[i * B_blockSize + k] +=
            A_block[i * n2 + j] * B_block[j * B_blockSize + k];
}
// Gather matrix C of grid blocks
void GatherC(const double *C_block, double *C, int A_blockSize, int B_blockSize,
             int aligned_n1, int aligned_n3, int countProc, MPI_Comm commGrid) {
  MPI_Datatype notResizedRecv_t;
  MPI_Datatype resizedRecv_t;

  int xDims = aligned_n1 / A_blockSize;
  int yDims = aligned_n3 / B_blockSize;
  int *recvCounts = malloc(sizeof(int) * countProc);
  int *displs = malloc(sizeof(int) * countProc);

  MPI_Type_vector(A_blockSize, B_blockSize, aligned_n3, MPI_DOUBLE,
                  &notResizedRecv_t);
  MPI_Type_commit(&notResizedRecv_t);

  MPI_Type_create_resized(notResizedRecv_t, 0, B_blockSize * sizeof(double),
                          &resizedRecv_t);
  MPI_Type_commit(&resizedRecv_t);

  for (int i = 0; i < xDims; ++i)
    for (int j = 0; j < yDims; ++j) {
      recvCounts[i * yDims + j] = 1;
      displs[i * yDims + j] = j + i * yDims * A_blockSize;
    }

  MPI_Gatherv(C_block, A_blockSize * B_blockSize, MPI_DOUBLE, C, recvCounts,
              displs, resizedRecv_t, 0, commGrid);

  MPI_Type_free(&notResizedRecv_t);
  MPI_Type_free(&resizedRecv_t);
  free(recvCounts);
  free(displs);
}

// Сheck result of multiplying matrices A and B stored in matrix C for
// correctness
bool CheckC(const double *C, int column, int leadingRow, int leadingColumn,
            int n2) {
  for (int i = 0; i < leadingRow; ++i)
    for (int j = 0; j < leadingColumn; ++j)
      if (C[i * column + j] != (double)i * j * n2) {
        printf("(%d, %d)\n", i, j);
        printf("%lf != %lf\n", C[i * column + j], (double)i * j * n2);
        return false;
      }

  return true;
}

int main(int argc, char **argv) {
  int n1 = 2000;
  int n2 = 1500;
  int n3 = 2500;
  int rankProc;
  int countProc;
  int aligned_n1;
  int aligned_n3;
  int A_blockSize;
  int B_blockSize;
  int dims[DIMS_COUNT] = {};
  int coords[DIMS_COUNT] = {};
  double startTime;
  double finishTime;
  double *A = NULL;
  double *B = NULL;
  double *C = NULL;
  double *A_block = NULL;
  double *B_block = NULL;
  double *C_block = NULL;
  MPI_Comm commGrid;
  MPI_Comm commRows;
  MPI_Comm commColumns;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &countProc);

  InitDims(dims, countProc, argc, argv);

  InitCommunicators(dims, &commGrid, &commRows, &commColumns);

  // Get coordinates of processes
  MPI_Cart_coords(commGrid, rankProc, DIMS_COUNT, coords);
  // Set parameters of matrix blocks
  A_blockSize = ceil((double)n1 / dims[X]);
  B_blockSize = ceil((double)n3 / dims[Y]);
  aligned_n1 = A_blockSize * dims[X];
  aligned_n3 = B_blockSize * dims[Y];

  if (coords[X] == 0 && coords[Y] == 0) {
    A = malloc(sizeof(double) * aligned_n1 * n2);
    B = malloc(sizeof(double) * n2 * aligned_n3);
    C = malloc(sizeof(double) * aligned_n1 * aligned_n3);

    GenerateMatrix(A, n2, n1, n2, true);
    GenerateMatrix(B, aligned_n3, n2, n3, false);
  }

  startTime = MPI_Wtime();

  A_block = malloc(sizeof(double) * A_blockSize * n2);
  B_block = malloc(sizeof(double) * B_blockSize * n2);
  C_block = malloc(sizeof(double) * A_blockSize * B_blockSize);

  SplitA(A, A_block, A_blockSize, n2, coords[Y], commRows, commColumns);
  SplitB(B, B_block, B_blockSize, n2, aligned_n3, coords[X], commRows,
         commColumns);

  Multi(A_block, B_block, C_block, A_blockSize, B_blockSize, n2);

  GatherC(C_block, C, A_blockSize, B_blockSize, aligned_n1, aligned_n3,
          countProc, commGrid);

  finishTime = MPI_Wtime();

  if (coords[Y] == 0 && coords[X] == 0) {
    printf("Is matrix C correct? - %s\n",
           CheckC(C, aligned_n3, n1, n3, n2) ? "yes" : "no");
    printf("Time: %lf\n", finishTime - startTime);

    free(A);
    free(B);
    free(C);
  }

  free(A_block);
  free(B_block);
  free(C_block);
  MPI_Comm_free(&commGrid);
  MPI_Comm_free(&commRows);
  MPI_Comm_free(&commColumns);

  MPI_Finalize();

  return EXIT_SUCCESS;
}
