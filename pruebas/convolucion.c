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


void convolve(double* signal, size_t signalLen, double* filter, size_t filterLen, double* result){
    size_t n;

    size_t kmin, kmax, k;

    for (n = 0; n < signalLen + filterLen - 1; n++)
    {
        result[n] = 0;

        //Optimización del recorrer.
        kmin = (n >= filterLen - 1) ? n - (filterLen - 1) : 0;
        kmax = (n < signalLen - 1) ? n : signalLen - 1;

        //printf("%d kmin, %d kmax\n",(int)kmin,(int)kmax);

        for (k = kmin; k <= kmax; k++)
        {
            result[n] += signal[k] * filter[n - k];
        }
    }
}

void convolve_index(double* signal, size_t signalStart, size_t signalEnd, double* filter, size_t filterLen, double* result/*inicializado a 0*/){
    size_t n;

    size_t kmin, kmax, k;

    size_t signalLen = signalEnd-signalStart;


    if(signalLen <= filterLen){
        printf("Filter size(%d) must be smaller than Signal size(%d)\n",(int)filterLen,(int)signalLen);
        exit(0);
    }

    for (n = signalStart; n < signalEnd + filterLen - 1; n++)
    {
        //result[n] = 0;

        //Optimización del recorrer.
        kmin = (n >= filterLen - 1) ? n - (filterLen - 1) : 0;
        kmax = (n < signalEnd - 1) ? n : signalEnd - 1;

        //printf("\t===> %d kmin, %d kmax\n",(int)kmin,(int)kmax);

        for (k = kmin; k <= kmax; k++)
        {
            result[n] += signal[k] * filter[n - k];
            //printf("Result = %f \t==>\t operacion = %f\n",result[n], signal[k] * filter[n - k]);
        }
    }
}


void input_side_conv(double *x, size_t lx, double *h, size_t lh, double *y)
{
    size_t i,j;
    for(i=0;i<lx; i++)
        for(j=0;j<lh;j++)
            y[i+j]=y[i+j]+x[i]*h[j];
}

void convNormalIndex(double* x, size_t signalStart, size_t signalEnd, double *h, size_t lh, double y[])
{
    size_t i,j;
    size_t lx = signalEnd-signalStart;
    //printf("ss = %d,se = %d,lx = %d\n",(int)signalStart,(int)signalEnd,(int)lx);

    for(i=signalStart;i<signalEnd; i++){
        //printf("%d \n",(int)i);
        for(j=0;j<lh;j++){
            y[i+j]=y[i+j]+x[i]*h[j];
            //printf("%d ==============> %f\n",(int)i,y[i]);
        }
        //printf("i = %d\n",(int)i);
    }

}

void convOverlapAddAllSound(double* x, size_t lx, double* h, size_t lh, double *y, size_t l) {

    if(lx <= lh){
        printf("H size (%d) must be smaller than X size (%d)\n",(int)lh,(int)lx);
        exit(0);
    }
    if(l <= lh){
        printf("H size must be smaller than L size\n");
        exit(0);
    }

    size_t lo = (size_t) pow(2, ceil(log(l+lh)/log(2)));
    //printf("size = %d\n",(int)lo);
    double* localBuffer = (double *) calloc(lo, sizeof(double));

    size_t ly = lx + lh - 1;

    size_t i = 0;
    size_t j;
    while(i <= lx){
        //printf("i = %d, lx = %d\n",(int)i,(int)lx);
        convNormalIndex(x, i, i+l, h, lh, y);

        // convNormalIndex(x, i, i+l, h, lh, localBuffer);

        /*for (j = 0; j < l; ++j) {

        }*/

        i+=l;
    }
}

int nextpw2(int valor){
    return  pow(2, ceil(log(valor)/log(2)));
}

void convOverlapAdd(double* partX, size_t lx, double localBuffer[], size_t lb/*potencia de 2*/, double* h, size_t lh, double y[]) {

    size_t i,j = 0;

    if(lx <= lh){
        printf("H size (%d) must be smaller than X size (%d)\n",(int)lh,(int)lx);
        exit(0);
    }

    convNormalIndex(partX, 0, lx, h, lh, localBuffer);

    for (j = 0; j < lx; j++) {
        y[j]= localBuffer[j];
        //printf("%d => %f\t%f \n",(int)j,localBuffer[j],y[j]);
        localBuffer[j] = 0;

    }

    for (i = 0; i < lb; i++) {
        localBuffer[i] = localBuffer[j];
        localBuffer[j] = 0;
        j++;
    }

}

#define COUNT(X) (sizeof(X) / sizeof((X)[0]))

int main(void)
{
    clock_t tic,toc;

    double signal[] = { 3,4,5,6,7,8,9,10};
    double filter[] = { 2,1 };
    //double result[COUNT(signal) + COUNT(filter) - 1] = NULL;
    double* result = (double *) calloc(COUNT(signal) + COUNT(filter) - 1, sizeof(double));

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
    result = (double *) calloc(COUNT(signal) + COUNT(filter) - 1, sizeof(double));

    tic = clock();
    //convolve_index(signal,0, COUNT(signal), filter, COUNT(filter), result);

    convNormalIndex(signal,0, COUNT(signal), filter, COUNT(filter), result);

    toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);

    //printf("Resultado Final:\n");
    for( j=0; j<(COUNT(signal) + COUNT(filter) - 1); j++ ) {
        printf("%f ",result[j]);
    }
    printf("\n");

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    result = (double *) calloc(COUNT(signal) + COUNT(filter) - 1, sizeof(double));

    int block_size = 4;
    int l_localBuffer = nextpw2(block_size+COUNT(filter)-1);
    double* localBuffer = (double *) calloc(l_localBuffer, sizeof(double));
    printf("size = %d\n",l_localBuffer);

    tic = clock();
    for( j=0; j<(COUNT(signal)/block_size); j++ ) {
        //printf("Bucle externo j: %d\n",j);
        //que empiece enl puntero en el inicio del vector signal o result pero en (j*block_size)
        convOverlapAdd(&signal[j*block_size], block_size, localBuffer, l_localBuffer, filter, COUNT(filter), &result[j*block_size]);
    }

    toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    free(localBuffer);
    printf("Resultado Final:\n");
    for( j=0; j<(COUNT(signal) + COUNT(filter) - 1); j++ ) {
        printf("%f ",result[j]);
    }
    printf("\n");

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("%d\n",(int)resultLength);

    double *A = malloc( resultLength*sizeof(double) );
    //double *B = malloc( resultLength*sizeof(double) );
    //double *C = (double *) calloc(resultLength, sizeof(double));


    /* datos aleatorios */
    /*    printf("[");
    int j;
    for( j=0; j<signalLength; j++ ) {
        A[j] = ((double) rand()/ RAND_MAX);
        printf("%f, ",A[j]);
    }
    printf("]\n");
     */
/*
    tic = clock();
    convolve(soundRandom, signalLength, H_1, filterLength, A);
    toc = clock();
    printf("Elapsed: %f miliseconds\n", (double)(toc - tic)*1000 / CLOCKS_PER_SEC);


    int block_size2 = 512;
    int l_localBuffer2 = nextpw2(block_size2+filterLength-1);
    double *localBuffer2 = (double *) calloc(l_localBuffer2, sizeof(double));
    printf("size = %d\n",l_localBuffer2);

    tic = clock();
    for( j=0; j<(signalLength/block_size2); j++ ) {
        printf("Bucle externo j: %d, indice:%d\n",j,j*block_size2);
        //que empiece enl puntero en el inicio del vector signal o result pero en (j*block_size)
        //convOverlapAdd(&soundRandom[j*block_size], block_size2, localBuffer2, l_localBuffer2, H_1, filterLength, &C[j*block_size]);
    }


    toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
*/

/*
    printf("A\tB\tC\n");
    //int j;
    for( j=0; j<resultLength; j++ ) {
        if(A[j]-C[j] != 0){
            printf("%f\t%f\n ",A[j],C[j]);
        }
    }
*/
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
