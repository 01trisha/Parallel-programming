#include <math.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

 // Initial coordinates
#define X_0 (double)-1.0
#define Y_0 (double)-1.0
#define Z_0 (double)-1.0

// Dimension size
#define D_X (double)2.0
#define D_Y (double)2.0
#define D_Z (double)2.0

// Grid size
#define N_X 400
#define N_Y 400
#define N_Z 400

// Step size
#define H_X (D_X / (N_X - 1))
#define H_Y (D_Y / (N_Y - 1))
#define H_Z (D_Z / (N_Z - 1))

// Square of step size
#define H_X_2 (H_X * H_X)
#define H_Y_2 (H_Y * H_Y)
#define H_Z_2 (H_Z * H_Z)

// Parameters
#define A (double)1.0E5
#define EPSILON (double)1.0E-3


double Phi(double x, double y, double z) {
    return x * x + y * y + z * z;
}


double Rho(double x, double y, double z) {
    return 6 - A * Phi(x, y, z);
}


int GetIndex(int x, int y, int z) {
    return x * N_Y * N_Z + y * N_Z + z;
}


double GetX(int i) {
    return X_0 + i * H_X;
}


double GetY(int j) {
    return Y_0 + j * H_Y;
}


double GetZ(int k) {
    return Z_0 + k * H_Z;
}
void SplitAreaIntoLayers(int* layerHeights, int* offsets, int countProc) {
    int offset = 0;
    for (int i = 0; i < countProc; ++i) {
        layerHeights[i] = N_X / countProc;

        // Distribute the remainder of the processes
        if (i < N_X % countProc)
            layerHeights[i]++;

        offsets[i] = offset;
        offset += layerHeights[i];
    }
}


void InitLayers(double* prevFunc, double* currFunc, int layerHeight, int offset) {
    for (int i = 0; i < layerHeight; ++i)
        for (int j = 0; j < N_Y; j++)
            for (int k = 0; k < N_Z; k++) {
                bool isBorder = (offset + i == 0) || (j == 0) || (k == 0) ||
                    (offset + i == N_X - 1) || (j == N_Y - 1) || (k == N_Z - 1);
                if (isBorder) {
                    prevFunc[GetIndex(i, j, k)] = Phi(GetX(offset + i), GetY(j), GetZ(k));
                    currFunc[GetIndex(i, j, k)] = Phi(GetX(offset + i), GetY(j), GetZ(k));
                }
                else {
                    prevFunc[GetIndex(i, j, k)] = 0;
                    currFunc[GetIndex(i, j, k)] = 0;
                }
            }
}


void SwapFunc(double** prevFunc, double** currFunc) {
    double* tmp = *prevFunc;
    *prevFunc = *currFunc;
    *currFunc = tmp;
}


double CalcCenter(const double* prevFunc, double* currFunc, int layerHeight, int offset) {
    double f_i = 0.0;
    double f_j = 0.0;
    double f_k = 0.0;
    double tmpMaxDiff = 0.0;
    double maxDiff = 0.0;

    for (int i = 1; i < layerHeight - 1; ++i)
        for (int j = 1; j < N_Y - 1; ++j)
            for (int k = 1; k < N_Z - 1; ++k) {
                f_i = (prevFunc[GetIndex(i + 1, j, k)] + prevFunc[GetIndex(i - 1, j, k)]) / H_X_2;
                f_j = (prevFunc[GetIndex(i, j + 1, k)] + prevFunc[GetIndex(i, j - 1, k)]) / H_Y_2;
                f_k = (prevFunc[GetIndex(i, j, k + 1)] + prevFunc[GetIndex(i, j, k - 1)]) / H_Z_2;

                currFunc[GetIndex(i, j, k)] =
                    (f_i + f_j + f_k - Rho(GetX(offset + i), GetY(j), GetZ(k))) / (2 / H_X_2 + 2 / H_Y_2 + 2 / H_Z_2 + A);

                // Update max difference
                tmpMaxDiff = fabs(currFunc[GetIndex(i, j, k)] - prevFunc[GetIndex(i, j, k)]);
                if (tmpMaxDiff > maxDiff)
                    maxDiff = tmpMaxDiff;
            }

    return maxDiff;
}
double CalcLimit(const double* prevFunc, double* currFunc, const double* upLimitLayer, const double* downLimitLayer, int layerHeight, int offset, int rankProc, int countProc) {
    double f_i = 0.0;
    double f_j = 0.0;
    double f_k = 0.0;
    double tmpMaxDiff = 0.0;
    double maxDiff = 0.0;

    for (int j = 1; j < N_Y - 1; ++j)
        for (int k = 1; k < N_Z - 1; ++k) {
            // Calculate the upper border
            if (rankProc != 0) {
                f_i = (prevFunc[GetIndex(1, j, k)] + upLimitLayer[GetIndex(0, j, k)]) / H_X_2;
                f_j = (prevFunc[GetIndex(0, j + 1, k)] + prevFunc[GetIndex(0, j - 1, k)]) / H_Y_2;
                f_k = (prevFunc[GetIndex(0, j, k + 1)] + prevFunc[GetIndex(0, j, k - 1)]) / H_Z_2;

                currFunc[GetIndex(0, j, k)] =
                    (f_i + f_j + f_k - Rho(GetX(offset), GetY(j), GetZ(k))) / (2 / H_X_2 + 2 / H_Y_2 + 2 / H_Z_2 + A);

                // Update max difference
                tmpMaxDiff = fabs(currFunc[GetIndex(0, j, k)] - prevFunc[GetIndex(0, j, k)]);
                if (tmpMaxDiff > maxDiff)
                    maxDiff = tmpMaxDiff;
            }

            // Calculate the lower border
            if (rankProc != countProc - 1) {
                f_i = (prevFunc[GetIndex(layerHeight - 2, j, k)] + downLimitLayer[GetIndex(0, j, k)]) / H_X_2;
                f_j = (prevFunc[GetIndex(layerHeight - 1, j + 1, k)] + prevFunc[GetIndex(layerHeight - 1, j - 1, k)]) / H_Y_2;
                f_k = (prevFunc[GetIndex(layerHeight - 1, j, k + 1)] + prevFunc[GetIndex(layerHeight - 1, j, k - 1)]) / H_Z_2;

                currFunc[GetIndex(layerHeight - 1, j, k)] =
                    (f_i + f_j + f_k - Rho(GetX(offset + layerHeight - 1), GetY(j), GetZ(k))) / (2 / H_X_2 + 2 / H_Y_2 + 2 / H_Z_2 + A);

                // Check for calculation end
                tmpMaxDiff = fabs(currFunc[GetIndex(layerHeight - 1, j, k)] - prevFunc[GetIndex(layerHeight - 1, j, k)]);
                if (tmpMaxDiff > maxDiff)
                    maxDiff = tmpMaxDiff;
            }
        }

    return maxDiff;
}

double CalcMaxDiff(const double* currFunc, int layerHeight, int offset) {
    double tmpMaxDelta = 0.0;
    double maxProcDelta = 0.0;
    double maxDelta = 0.0;

    for (int i = 0; i < layerHeight; ++i)
        for (int j = 0; j < N_Y; ++j)
            for (int k = 0; k < N_Z; ++k) {
                tmpMaxDelta = fabs(currFunc[GetIndex(i, j, k)] - Phi(GetX(offset + i), GetY(j), GetZ(k)));
                if (tmpMaxDelta > maxProcDelta)
                    maxProcDelta = tmpMaxDelta;
            }

    
    MPI_Allreduce(&maxProcDelta, &maxDelta, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    return maxDelta;
}


int main(int argc, char** argv) {
    int rankProc = 0;
    int countProc = 0;
    double startTime = 0.0;
    double finishTime = 0.0;
    double maxDiff = 0.0;
    int* layerHeights = NULL;
    int* offsets = NULL;
    double* upLimitLayer = NULL;
    double* downLimitLayer = NULL;
    double* prevFunc = NULL;
    double* currFunc = NULL;
    MPI_Request sendUpReq;
    MPI_Request sendDownReq;
    MPI_Request recvUpReq;
    MPI_Request recvDownReq;

    // Check grid size
    if (N_X < 3 || N_Y < 3 || N_Z < 3) {
        fprintf(stderr, "Incorrect grid size\n");
        return EXIT_FAILURE;
    }

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    // Divide area
    layerHeights = malloc(sizeof(int) * countProc);
    offsets = malloc(sizeof(int) * countProc);
    SplitAreaIntoLayers(layerHeights, offsets, countProc);
    // Init layers
    prevFunc = malloc(sizeof(double) * layerHeights[rankProc] * N_Y * N_Z);
    currFunc = malloc(sizeof(double) * layerHeights[rankProc] * N_Y * N_Z);
    InitLayers(prevFunc, currFunc, layerHeights[rankProc], offsets[rankProc]);

    upLimitLayer = malloc(sizeof(double) * N_Y * N_Z);
    downLimitLayer = malloc(sizeof(double) * N_Y * N_Z);

    startTime = MPI_Wtime();
    do {
        double tmpMaxDiff = 0.0;
        double procMaxDiff = 0.0;

    // Swap functions
    SwapFunc(&prevFunc, &currFunc);

    // Start sending and receiving border
    if (rankProc != 0) {
        double* prevUpLimit = prevFunc;
        MPI_Isend(prevUpLimit, N_Y * N_Z, MPI_DOUBLE, rankProc - 1, rankProc, MPI_COMM_WORLD, &sendUpReq);
        MPI_Irecv(upLimitLayer, N_Y * N_Z, MPI_DOUBLE, rankProc - 1, rankProc - 1, MPI_COMM_WORLD, &recvUpReq);
    }
    if (rankProc != countProc - 1) {
    double* prevDownLimit = prevFunc + (layerHeights[rankProc] - 1) * N_Y * N_Z;
    MPI_Isend(prevDownLimit, N_Y * N_Z, MPI_DOUBLE, rankProc + 1, rankProc, MPI_COMM_WORLD, &sendDownReq);
    MPI_Irecv(downLimitLayer, N_Y * N_Z, MPI_DOUBLE, rankProc + 1, rankProc + 1, MPI_COMM_WORLD, &recvDownReq);
}

    // Calculate center
    tmpMaxDiff = CalcCenter(prevFunc, currFunc, layerHeights[rankProc], offsets[rankProc]);

    // Finish sending and receiving border
    if (rankProc != 0) {
        MPI_Wait(&sendUpReq, MPI_STATUS_IGNORE);
        MPI_Wait(&recvUpReq, MPI_STATUS_IGNORE);
    }

    if (rankProc != countProc - 1) {
        MPI_Wait(&sendDownReq, MPI_STATUS_IGNORE);
        MPI_Wait(&recvDownReq, MPI_STATUS_IGNORE);
    }

    // Calculate border
    procMaxDiff = CalcLimit(prevFunc, currFunc, upLimitLayer, downLimitLayer,
        layerHeights[rankProc], offsets[rankProc], rankProc, countProc);
    procMaxDiff = fmax(tmpMaxDiff, procMaxDiff);

    // Calculate the differences of the previous and current calculated functions
    MPI_Allreduce(&procMaxDiff, &maxDiff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
} while (maxDiff >= EPSILON);

    // Calculate the differences of the calculated and theoretical functions
    maxDiff = CalcMaxDiff(currFunc, layerHeights[rankProc], offsets[rankProc]);

    finishTime = MPI_Wtime();

    if (rankProc == 0) {
        printf("Time: %lf\n", finishTime - startTime);
        printf("Max difference: %lf\n", maxDiff);
    }

    free(offsets);
    free(layerHeights);
    free(prevFunc);
    free(currFunc);
    free(upLimitLayer);
    free(downLimitLayer);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
