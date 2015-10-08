#include <stdio.h>
#include <omp.h>

//time
#include <time.h>

void subdomain(float *x, int istart, int ipoints)
{
    int i;
    for (i = 0; i < ipoints; i++)
        x[istart+i] = 123.456*123/1968*1968+196851*56;
}
void sub(float *x, int npoints)
{
    int iam, nt, ipoints, istart;
    #pragma omp parallel default(shared) private(iam,nt,ipoints,istart)
    {
        iam = omp_get_thread_num();
        nt = omp_get_num_threads();
        ipoints = npoints / nt; /* size of partition */
        istart = iam * ipoints; /* starting array index */

        printf("%d, %d\n",iam, nt);

        if (iam == nt-1) /* last thread may do more */
            ipoints = npoints - istart;
        subdomain(x, istart, ipoints);
    }
}
int main() {
    clock_t tic,toc;
    //omp_set_num_threads(1);

    tic = clock();

    float array[1000000];
    sub(array, 1000000);

    toc = clock();
    printf("Elapsed: %f miliseconds\n", (double)(toc - tic)*1000 / CLOCKS_PER_SEC);

    return 0;
}


/*
int main(void)
{
    int a[1000000];
    int i;

    int nthreads, tid;
#pragma omp parallel private(tid)

    tid = omp_get_thread_num();
    printf("Hello World from thread = %d\n", tid);

    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);

    #pragma omp parallel for
    for (i = 0; i < 1000000; i++){
        a[i] = 2 * i;
        //printf("%d,%d\n",i,a[i]);
    }


    #pragma omp parallel
    printf("Hello, world.\n");
    return 0;
}
*/