// $ gcc -fsanitize=address -g overlapADD.c -lm -O3

#include <stddef.h>
#include <stdio.h>

//exit(0), malloc, calloc
#include <stdlib.h>

//time
#include <time.h>

//ceil
#include <math.h>
#include <string.h>

//OpenMP
#include <omp.h>

//importar muestra
#include "../import/muestra.h"

//#define COUNT(X) (sizeof(X) / sizeof((X)[0]))

int nextpw2(int valor){
    return  pow(2, ceil(log(valor)/log(2)));
}

void convNormalIndex(double* x, size_t signalStart, size_t signalEnd, double *h, size_t lh, double y[])
{
    size_t i,j;
    size_t lx = signalEnd-signalStart;

    for(i=signalStart;i<signalEnd; i++){
        for(j=0;j<lh;j++){
             y[i+j]=y[i+j]+x[i]*h[j];
        }
    }
}

void convOverlapAdd(double* partX, size_t lx, double localBuffer[], size_t lb/*potencia de 2*/, double* h, size_t lh, double y[]) {

    size_t i,j = 0;

    convNormalIndex(partX, 0, lx, h, lh, localBuffer);

    for (j = 0; j < lx; j++) {
        y[j]= localBuffer[j];
        localBuffer[j] = 0;
    }

    for (i = 0; i < lx; i++) {
        localBuffer[i] = localBuffer[j];
        localBuffer[j] = 0;
        j++;
    }
}

void convNormalIndex_P(double* x, size_t signalStart, size_t signalEnd, double *h, size_t lh, double y[])
{
    size_t i,j;
    size_t lx = signalEnd-signalStart;

    int i_am, nt, l_local, istart;

#pragma omp parallel default(shared) private(i_am,nt,l_local,istart)
    {
        i_am = omp_get_thread_num();
        nt = omp_get_num_threads();
        l_local = lx / nt; /* size of partition */
        istart = i_am * l_local; /* starting array index */

        //if (l_local <= lh) /* size_local <= h_size error*/
            //ipoints = npoints - istart;
        //subdomain(x, istart, ipoints);
    }


//#pragma omp parallel for default(none) shared(i,j,x,lx, signalStart, signalEnd, h, lh, y)
    for(i=signalStart;i<signalEnd; i++){
        for(j=0;j<lh;j++){
            y[i+j]=y[i+j]+x[i]*h[j];
        }
    }
}


void convOverlapAdd_P(double* partX, size_t lx, double localBuffer[], size_t lb/*potencia de 2*/, double* h, size_t lh, double y[]) {

    size_t i,j = 0;

    convNormalIndex_P(partX, 0, lx, h, lh, localBuffer);

    //#pragma omp parallel for default(shared)
    for (j = 0; j < lx; j++) {
        y[j]= localBuffer[j];
        localBuffer[j] = 0;
    }

    //#pragma omp parallel for default(shared)
    for (i = 0; i < lx; i++) {
        localBuffer[i] = localBuffer[j];
        localBuffer[j] = 0;
        j++;
    }
}