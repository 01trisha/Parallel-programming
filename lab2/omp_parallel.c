#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5000
#define EPSILON 0.00001
#define TAU 0.001
#define MAX_ITERATION_COUNT 10000000

//делим матрицу на части между потоками (кол-во обрабатываемых строк, с какой строки начинать потоку)
void SetMatrixPart(int* lineNum, int* lineOffsets, int size, int stringNum)
{
    int offset = 0;
    for (int i = 0; i < stringNum; ++i)
    {
        lineNum[i] = size / stringNum;

        if (i < size % stringNum)
            ++lineNum[i];

        lineOffsets[i] = offset;
        offset += lineNum[i];
    }
}


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
    for (int i = 0; i < size; ++i)
        normSquare += vector[i] * vector[i];

    return normSquare;
}

void GetAxb(const double* A, const double* x, const double* b, double* Axb, int size)
{
    for (int i = 0; i < size; ++i)
    {
        Axb[i] = -b[i];
        for (int j = 0; j < N; ++j)
            Axb[i] += A[i * N + j] * x[j];
    }
}


void GetNextX(const double* Axb, double* x, double tau, int size)
{
    for (int i = 0; i < size; ++i)
        x[i] -= tau * Axb[i];
}


int main(int argc, char** argv)
{
    int iterNum = 0;
    int stringId;
    int stringNum = omp_get_max_threads();
    double accuracy = EPSILON + 1;
    double normB;
    double startTime;
    double finishTime;
    int* lineNum = malloc(sizeof(int) * stringNum);
    int* lineOffsets = malloc(sizeof(int) * stringNum);
    double* A = malloc(sizeof(double) * N * N);
    double* x = malloc(sizeof(double) * N);
    double* b = malloc(sizeof(double) * N);
    double* Axb = malloc(sizeof(double) * N);

    SetMatrixPart(lineNum, lineOffsets, N, stringNum);

    CreateA(A, N);
    CreateX(x, N);
    CreateB(b, N);

    normB = sqrt(GetNormSquare(b, N));

    startTime = omp_get_wtime();
//создаем параллельную область и каждому потоку передаем свой идентификатор(если будет общей то потоки будут менять ее одновременно)
#pragma omp parallel private(stringId)
    {
        stringId = omp_get_thread_num();
        for (iterNum = 0; accuracy > EPSILON && iterNum < MAX_ITERATION_COUNT; ++iterNum)
        {
            GetAxb(A + lineOffsets[stringId] * N, x, b + lineOffsets[stringId], Axb + lineOffsets[stringId], lineNum[stringId]);
//заставляем ждать потоки друг друга чтоб все потоки завершили считать до обновления x
#pragma omp barrier

            GetNextX(Axb + lineOffsets[stringId], x + lineOffsets[stringId], TAU, lineNum[stringId]);
//чарантирует, что блок кода внутри нее будет выполнен только одним потоком, а остальные потоки будут ждать его завершения 
#pragma omp single
            accuracy = 0;
//гарантируем атомарное сложение(без вмещательства других потоков) 
#pragma omp atomic
            accuracy += GetNormSquare(Axb + lineOffsets[stringId], lineNum[stringId]);
//заставляем потоки ждать друг друга чтоб все потоки завершили считать до обновления accuracy
#pragma omp barrier
//арантирует, что блок кода внутри нее будет выполнен только одним потоком, а остальные потоки будут ждать его завершения 
#pragma omp single
            accuracy = sqrt(accuracy) / normB;
        }
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
