#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NPOINTS 2000
#define MAXITER 2000

struct d_complex { double r, i; };

void testpoint(struct d_complex c, int *outside) {
    struct d_complex z = c;
    for (int iter = 0; iter < MAXITER; iter++) {
        double temp = (z.r * z.r) - (z.i * z.i) + c.r;
        z.i = 2.0 * z.r * z.i + c.i;
        z.r = temp;
        if ((z.r * z.r + z.i * z.i) > 4.0) { (*outside)++; break; }
    }
}

int main() {
    int thread_counts[] = {1, 2, 3, 4};
    for (int t = 0; t < 4; t++) {
        int num_threads = thread_counts[t];
        omp_set_num_threads(num_threads);
        int numoutside = 0;

        double tstart = omp_get_wtime();

        #pragma omp parallel for reduction(+:numoutside) schedule(dynamic, 10)
        for (int i = 0; i < NPOINTS; i++) {
            for (int j = 0; j < NPOINTS; j++) {
                struct d_complex c = {-2.0 + (2.5 * i) / NPOINTS, (1.125 * j) / NPOINTS};
                int out = 0;
                testpoint(c, &out);
                if (out) numoutside++;
            }
        }

        double tstop = omp_get_wtime();
        double area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
        printf("Threads: %d | Area: %.6f | Time: %.3f ms\n", num_threads, area, (tstop - tstart) * 1000.0);
    }
    return 0;
}
