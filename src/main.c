// $ gcc -fsanitize=address -g overlapADD.c -lm

#include <stddef.h>
#include <stdio.h>

//importar convolution
#include "convolution.h"

#define COUNT(X) (sizeof(X) / sizeof((X)[0]))


int main(void) {
    clock_t tic,toc;
    //Filtro H_1 cargado
    //filterLength = 61;

    int j,z;
    int tamX = 10000;
    int tamResultado = tamX+filterLength;

    //vector señal
    double *A = malloc( tamX * sizeof(double) );

    //vector resultado convolucion normal
    double *B = malloc( tamResultado*sizeof(double) );

    //tamaño de bloque
    int block_size = 512;

    //tamaño buffer local a la siguiente potencia de 2
    int l_localBuffer = nextpw2(block_size+filterLength-1);
    printf("size = %d\n",l_localBuffer);

    //vector buffer local
    double* localBuffer = (double *) calloc(l_localBuffer, sizeof(double));

    //vector resultado convolucion ovelap-add + un poco más para guardar la ultima parte del vector que falta
    double *C = (double *) calloc(tamResultado + block_size, sizeof(double));

    //vector de ultima pasada con valores 0
    double * lastPass = (double *) calloc(block_size, sizeof(double));

    /* generar datos aleatorios para el vector A*/
    //printf("[");
    for( j=0; j<tamX; j++ ) {
        A[j] = ((double) rand()/ RAND_MAX);
        //printf("%f, ",A[j]);
    }
    //printf("]\n");


    tic = clock();
    convNormalIndex(A,0,tamX,H_1,filterLength,B);
    toc = clock();
    printf("Elapsed: %f miliseconds\n", (double)(toc - tic)*1000 / CLOCKS_PER_SEC);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    tic = clock();

    for( j=0; j<(tamX/block_size); j++ ) {
        convOverlapAdd(&A[j*block_size], block_size, localBuffer, l_localBuffer, H_1, filterLength, &C[j*block_size]);
    }
    //Añadir ultimo recorrido.
    int last_index = j*block_size;
    int sizeleft = tamX - last_index;

    if( sizeleft != 0 ){
        for (z = 0; z < sizeleft; ++z) {
            lastPass[z] = A[z+last_index];
        }
        convOverlapAdd(&lastPass[0], block_size, localBuffer, l_localBuffer, H_1, filterLength, &C[last_index]);
    }

    toc = clock();
    printf("Elapsed: %f miliseconds\n", (double)(toc - tic)*1000 / CLOCKS_PER_SEC);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*    printf("A\tB\tC\n");
    //int j;
    for( j=0; j<(tamX+filterLength-1); j++ ) {
        if(A[j]-C[j] != 0){
            printf("%d => %f\t%f\n",(int)j,A[j],C[j]);
        }
    }
*/

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