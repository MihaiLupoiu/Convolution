// $ gcc -fsanitize=address -g overlapADD.c -lm
//

#include <stddef.h>
#include <stdio.h>

//importar convolution
#include "convolution.h"

#define COUNT(X) (sizeof(X) / sizeof((X)[0]))

void convNormalIndexNEON_v1(double* x, size_t signalStart, size_t signalEnd, double *h, size_t lh, double y[])
{
    size_t i,j;
    size_t lx = signalEnd-signalStart;

    double temp[4];

    for(i=signalStart;i<signalEnd; i++){
        //temp[0] = temp[1] = temp[2] = temp[3] = 0;

        for(j=0;j<lh;j+=4){

            temp[0] = x[i]*h[j+0];
            temp[1] = x[i]*h[j+1];
            temp[2] = x[i]*h[j+2];
            temp[3] = x[i]*h[j+3];

            y[i+j+0] += temp[0];
            y[i+j+1] += temp[1];
            y[i+j+2] += temp[2];
            y[i+j+3] += temp[3];

        }

    }
}

void convNormalIndexNEON_v2(double* x, size_t signalStart, size_t signalEnd, double *h, size_t lh, double y[])
{
    size_t i,j;
    size_t lx = signalEnd-signalStart;

    double temp[4];

    for(j=0;j<lh;j++){
        for(i=signalStart;i<signalEnd; i+=4){

            temp[0] = x[i+0]*h[j];
            temp[1] = x[i+1]*h[j];
            temp[2] = x[i+2]*h[j];
            temp[3] = x[i+3]*h[j];
            //printf("=====> i=%d,j=%d\t%f, %f, %f, %f\n",(int)i,(int)j,temp[0],temp[1],temp[2],temp[3] );
            y[i+j+0] += temp[0];
            y[i+j+1] += temp[1];
            y[i+j+2] += temp[2];
            y[i+j+3] += temp[3];
            //printf("i=%d,j=%d\t%f, %f, %f, %f\n",(int)i,(int)j,y[i+j+0],y[i+j+1],y[i+j+2],y[i+j+3]);
        }

    }
}

int main(void) {
    clock_t tic,toc;
    //Filtro H_1 cargado
    //filterLength = 61;

    int i;
    filterLength = 4;
    double* H1 = malloc( filterLength*sizeof(double) );
    for (i = 0; i < filterLength; ++i) {
        H1[i] = (double)(i+1);
    }

    int j,z;
    int tamX = 8;
    int tamResultado = tamX+filterLength;


    //vector señal
    //int *A = malloc( tamX * sizeof(int) );

    double A[] = {1,2,3,4,5,6,7,8};


    //vector resultado convolucion normal
    double *B = malloc( tamResultado*sizeof(double) );

    //tamaño de bloque
    int block_size = 4;

    //tamaño buffer local a la siguiente potencia de 2
    int l_localBuffer = nextpw2(block_size+filterLength-1);
    //printf("size = %d\n",l_localBuffer);

    //vector buffer local
    double* localBuffer = (double *) calloc(l_localBuffer, sizeof(double));

    //vector resultado convolucion ovelap-add + un poco más para guardar la ultima parte del vector que falta
    double *C = (double *) calloc(tamResultado + block_size, sizeof(double));

    //vector de ultima pasada con valores 0
    double * lastPass = (double *) calloc(block_size, sizeof(double));


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    tic = clock();
    convNormalIndex(A,0,tamX,H1,filterLength,B);
    toc = clock();
    printf("Elapsed: %f miliseconds\n", (double)(toc - tic)*1000 / CLOCKS_PER_SEC);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    tic = clock();
    //convNormalIndexNEON_v1(A,0,tamX,H1,filterLength,C);
    toc = clock();
    printf("Elapsed: %f miliseconds\n", (double)(toc - tic)*1000 / CLOCKS_PER_SEC);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    tic = clock();
    convNormalIndexNEON_v2(A,0,tamX,H1,filterLength,C);
    toc = clock();
    printf("Elapsed: %f miliseconds\n", (double)(toc - tic)*1000 / CLOCKS_PER_SEC);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    tic = clock();
    for(j=0; j<(tamX/block_size); j++ ) {
        convOverlapAdd(&A[j*block_size], block_size, localBuffer, l_localBuffer, H1, filterLength, &C[j*block_size]);
    }
    //Añadir ultimo recorrido.
    int last_index = j*block_size;
    int sizeleft = tamX - last_index;

    if( sizeleft != 0 ){
        for (z = 0; z < sizeleft; ++z) {
            lastPass[z] = A[z+last_index];
        }
        convOverlapAdd(&lastPass[0], block_size, localBuffer, l_localBuffer, H1, filterLength, &C[last_index]);
    }

    toc = clock();
    printf("Elapsed: %f miliseconds for the imput NEON\n", (double)(toc - tic)*1000 / CLOCKS_PER_SEC);

    j = 0;
    while(localBuffer[j] != 0){
        C[last_index+block_size+j]= localBuffer[j];
        localBuffer[j] = 0;
        j++;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

*/
    printf("A\tB\tC\n");
    //int j;
    for( j=0; j<(tamX+filterLength-1);j++ ) {
        if(B[j]-C[j] != 0){
            printf("%d\t%f\t",(int)j,B[j]);
            printf("%f\n",C[j]);
            //printf("\n");
        }
    }


/*    printf("[");
    for( j=0; j<resultLength+512; j++ ) {
        if (j == resultLength+512-1){
            printf("%f ",C[j]);
        }else{
            printf("%f, ",C[j]);
        }
    }
    printf("]\n");
*/
    return 0;
}