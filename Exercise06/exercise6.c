#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000
#define STRIP_SIZE 64

int main() {
    double *A = (double*)malloc(N * sizeof(double));
    double *B = (double*)malloc(N * sizeof(double));
    double *C = (double*)malloc(N * sizeof(double));

    for (int i = 0; i < N; i++) { A[i] = i * 1.0; B[i] = 2.0; }

    double tstart = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i += STRIP_SIZE) {
        int limit = (i + STRIP_SIZE > N) ? N : (i + STRIP_SIZE);
        #pragma omp simd
        for (int j = i; j < limit; j++) { C[j] = A[j] * B[j]; }
    }

    double tstop = omp_get_wtime();
    printf("Array Multiplication Complete | Time: %.3f ms\n", (tstop - tstart) * 1000.0);

    free(A); free(B); free(C);
    return 0;
}
