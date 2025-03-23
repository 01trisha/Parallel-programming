#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5000
#define EPSILON 1E-7
#define TAU 1E-5
#define MAX_ITERATION_COUNT 1000000


void CreateA(double* A, int size)
{
#pragma omp parallel for
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; ++j)
            A[i * size + j] = 1;

        A[i * size + i] = 2;
    }
}

void CreateX(double* x, int size)
{
#pragma omp parallel for
    for (int i = 0; i < size; i++)
        x[i] = 0;
}

void CreateB(double* b, int size)
{
#pragma omp parallel for
    for (int i = 0; i < size; i++)
        b[i] = N + 1;
}

double GetNormSquare(const double* vector, int size)
{
    double normSquare = 0.0;

#pragma omp parallel for reduction(+ : normSquare)
    for (int i = 0; i < size; ++i)
        normSquare += vector[i] * vector[i];

    return normSquare;
}

void GetAxb(const double* A, const double* x, const double* b, double* Axb, int size)
{
#pragma omp parallel for
    for (int i = 0; i < size; ++i)
    {
        Axb[i] = -b[i];
        for (int j = 0; j < N; ++j)
            Axb[i] += A[i * N + j] * x[j];
    }
}

void GetNextX(const double* Axb, double* x, double tau, int size)
{
#pragma omp parallel for
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
    



