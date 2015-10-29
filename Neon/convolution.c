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