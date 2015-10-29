#ifndef CONVOLUTION_H
#define CONVOLUTION_H

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

int nextpw2(int valor);
void convNormalIndex(double* x, size_t signalStart, size_t signalEnd, double *h, size_t lh, double y[]);
void convOverlapAdd(double* partX, size_t lx, double localBuffer[], size_t lb/*potencia de 2*/, double* h, size_t lh, double y[]);
#endif

