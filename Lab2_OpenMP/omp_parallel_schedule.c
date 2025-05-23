#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5000
#define EPSILON 0.00001
#define TAU 0.001
#define MAX_ITERATION_COUNT 10000000


void CreateA(double* A, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; ++j)
            A[i * size + j] = 1;

        A[i * size + i] = 2;
    }
}

void CreateX(double* x, int size)
{
    for (int i = 0; i < size; i++)
        x[i] = 0;
}

void CreateB(double* b, int size)
{
    for (int i = 0; i < size; i++)
        b[i] = N + 1;
}
double GetNormSquare(const double* vector, int size)
{
    double normSquare = 0.0;

#pragma omp parallel for schedule(runtime) reduction(+ : normSquare)
    for (int i = 0; i < size; ++i)
        normSquare += vector[i] * vector[i];

    return normSquare;
}

void GetAxb(const double* A, const double* x, const double* b, double* Axb, int size)
{
#pragma omp parallel for schedule(runtime)
    for (int i = 0; i < size; ++i)
    {
        Axb[i] = -b[i];
        for (int j = 0; j < N; ++j)
            Axb[i] += A[i * N + j] * x[j];
    }
}

void GetNextX(const double* Axb, double* x, double tau, int size)
{
#pragma omp parallel for schedule(runtime)
    for (int i = 0; i < size; ++i)
        x[i] -= tau * Axb[i];
}
int main(int argc, char** argv)
{
    int iterNum;
    double accuracy = EPSILON + 1;
    double normB;
    double startTime;
    double finishTime;
    double* A = malloc(sizeof(double) * N * N);
    double* x = malloc(sizeof(double) * N);
    double* b = malloc(sizeof(double) * N);
    double* Axb = malloc(sizeof(double) * N);

    CreateA(A, N);
    CreateX(x, N);
    CreateB(b, N);

    normB = sqrt(GetNormSquare(b, N));

    startTime = omp_get_wtime();

    for (iterNum = 0; accuracy > EPSILON && iterNum < MAX_ITERATION_COUNT; ++iterNum)
    {
        GetAxb(A, x, b, Axb, N);
        GetNextX(Axb, x, TAU, N);
        accuracy = sqrt(GetNormSquare(Axb, N)) / normB;
    }

    finishTime = omp_get_wtime();

    // printf("Матрица A:\n");
    // for (int i = 0; i < 10; ++i) {
    //     for (int j = 0; j < 10; ++j) {
    //         printf("%6.2lf ", A[i * N + j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // printf("Вектор B:\n");
    // for (int i = 0; i < 10; ++i) {
    //     printf("B[%d] = %lf\n", i, b[i]);
    // }
    // printf("\n");

    // printf("Вектор X:\n");
    // for (int i = 0; i < 10; ++i)
    //     printf("x[%d] = %lf\n", i, x[i]);


    if (iterNum == MAX_ITERATION_COUNT)
        printf("Too many iterations\n");
    else
    {
        printf("Time: %lf sec\n", finishTime - startTime);
    }

    free(A);
    free(x);
    free(b);
    free(Axb);

    return 0;
}
