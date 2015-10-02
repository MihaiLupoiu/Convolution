// $ gcc -fsanitize=address -g overlapADD.c -lm

#include <stddef.h>
#include <stdio.h>
//exit(0), malloc, calloc
#include <stdlib.h>
#include <time.h>
//ceil
#include <math.h>
#include <string.h>

//importar muestra
#include "../import/muestra.h"

int nextpw2(int valor){
    return  pow(2, ceil(log(valor)/log(2)));
}

void convNormalIndex(double* x, size_t signalStart, size_t signalEnd, double *h, size_t lh, double y[])
{
    size_t i,j;
    size_t lx = signalEnd-signalStart;
    //printf("ss = %d,se = %d,lx = %d\n",(int)signalStart,(int)signalEnd,(int)lx);

    for(i=signalStart;i<signalEnd; i++){
        for(j=0;j<lh;j++){
            //printf("i=%d, j=%d \n",(int)i,(int)j);
            //printf("(y[%d+%d]=%f)+(x[%d]=%f)*(h[%d]=%f)\t = ",(int)i,(int)j,y[i+j],(int)i,x[i],(int)j,h[j]);
            y[i+j]=y[i+j]+x[i]*h[j];
            //printf("Size of y: %d bytes\n",sizeof(y[i+j]));

            //printf("y[%d+%d]=%f\n",(int)i,(int)j,y[i+j]);
        }
    }

}

void convOverlapAdd(double* partX, size_t lx, double localBuffer[], size_t lb/*potencia de 2*/, double* h, size_t lh, double y[]) {

    size_t i,j = 0;

    if(lx <= lh){
        printf("H size (%d) must be smaller than X size (%d)\n",(int)lh,(int)lx);
        exit(0);
    }
    convNormalIndex(partX, 0, lx, h, lh, localBuffer);
/*
    for (j = 0; j < lb; j++) {
        printf("localBuffer[%d] = %f, \t",(int)j,localBuffer[j]);
        printf("MD_localBffer[%d] = %p\n",(int)j,&localBuffer[j]);
    }
*/
    for (j = 0; j < lx; j++) {
        y[j]= localBuffer[j];
        //printf("\tlocalBuffer[%d] = %f \t y[%d] = %f \n",(int)j,localBuffer[j],(int)j,y[j]);
        localBuffer[j] = 0;
    }

   for (i = 0; i < lx; i++) {
        localBuffer[i] = localBuffer[j];
        localBuffer[j] = 0;
        j++;
    }
/*
    for (j = 0; j < lb; j++) {
        printf("localBuffer[%d] = %f\t",(int)j,localBuffer[j]);
        printf("MD_localBffer[%d] = %p\n",(int)j,&localBuffer[j]);
    }
*/
}

#define COUNT(X) (sizeof(X) / sizeof((X)[0]))

int main(void)
{
    clock_t tic,toc;

    double signal[] = { 3,4,5,6,7,8,9,10};
    double filter[] = { 2,1 };
    int sizeSignal = 8;
    int sizeFilter = 2;
    double* result = (double *) calloc(sizeSignal + sizeFilter - 1, sizeof(double));

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    tic = clock();
    convolve(signal, COUNT(signal), filter, COUNT(filter), result);
    toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
*/
    int j;/*
    printf("Resultado Final:\n");
    for( j=0; j<(COUNT(signal) + COUNT(filter) - 1); j++ ) {
        printf("%f ",result[j]);
    }
    printf("\n");
*/
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    free(result);
    result = (double *) calloc(COUNT(signal) + COUNT(filter) - 1, sizeof(double));

    tic = clock();
    //convolve_index(signal,0, COUNT(signal), filter, COUNT(filter), result);

    convNormalIndex(signal,0, COUNT(signal), filter, COUNT(filter), result);

    toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);

    //printf("Resultado Final:\n");
    for( j=0; j<(sizeSignal + sizeFilter - 1); j++ ) {
        printf("%f ",result[j]);
    }
    printf("\n");

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    free(result);
    result = (double *) calloc(sizeSignal + sizeFilter - 1, sizeof(double));

    int block_size = 4;
    int l_localBuffer = nextpw2(block_size+sizeFilter-1);
    double* localBuffer = (double *) calloc(l_localBuffer, sizeof(double));
/*    printf("size = %d\n",l_localBuffer);

    for( j=0; j<l_localBuffer; j++ ) {
        printf("MD_localBffer[%d] = %p\n",(int)j,&localBuffer[j]);
    }

    for( j=0; j<(sizeSignal + sizeFilter - 1); j++ ) {
        printf("MD_result[%d] = %p\n",(int)j,&result[j]);
    }
*/

    tic = clock();
    for( j=0; j<(sizeSignal/block_size); j++ ) {
        //printf("Bucle externo j: %d\n",j);
        //que empiece enl puntero en el inicio del vector signal o result pero en (j*block_size)
        convOverlapAdd(&signal[j*block_size], block_size, localBuffer, l_localBuffer, filter, sizeFilter, &result[j*block_size]);
    }

    toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    printf("Resultado Final:\n");
    for( j=0; j<(sizeSignal + sizeFilter - 1); j++ ) {
        printf("%f ",result[j]);
        //printf("%f \t",result[j]);
        //printf("MD_result[%d] = %p\n",(int)j,&result[j]);

    }
    printf("\n");
    free(localBuffer);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    double *A = malloc( resultLength*sizeof(double) );
    double *B = malloc( resultLength*sizeof(double) );
    double *C = (double *) calloc(resultLength, sizeof(double));


    /* datos aleatorios */
    /*    printf("[");
    int j;
    for( j=0; j<signalLength; j++ ) {
        A[j] = ((double) rand()/ RAND_MAX);
        printf("%f, ",A[j]);
    }
    printf("]\n");
     */

    tic = clock();
    convNormalIndex(soundRandom,0,signalLength,H_1,filterLength,A);
    toc = clock();
    printf("Elapsed: %f miliseconds\n", (double)(toc - tic)*1000 / CLOCKS_PER_SEC);


    block_size = 512;
    l_localBuffer = nextpw2(block_size+filterLength-1);
    localBuffer = (double *) calloc(l_localBuffer, sizeof(double));
    printf("size = %d\n",l_localBuffer);

    tic = clock();
    for( j=0; j<(signalLength/block_size); j++ ) {
        //printf("Bucle externo j: %d, indice:%d\n",j,j*block_size);
        //que empiece enl puntero en el inicio del vector signal o result pero en (j*block_size)
        convOverlapAdd(&soundRandom[j*block_size], block_size, localBuffer, l_localBuffer, H_1, filterLength, &C[j*block_size]);
    }
    toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);


    printf("A\tB\tC\n");
    //int j;
    for( j=0; j<block_size; j++ ) {
        if(A[j]-C[j] != 0){
            printf("%f\t%f\n ",A[j],C[j]);
        }
    }

/*
    printf("[");
    int j;
    for( j=0; j<resultLength; j++ ) {
        if (j == resultLength-1){
            printf("%f ",C[j]);
        }else{
            printf("%f, ",C[j]);
        }
    }
    printf("]\n");
*/
    return 0;
}
