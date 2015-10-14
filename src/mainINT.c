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

    int i;
    filterLength = 100;
    double* H1 = malloc( filterLength*sizeof(double) );
    for (i = 0; i < filterLength; ++i) {
        H1[i] = (double)(i+1);
    }

    int j,z;
    int tamX = 1000;
    int tamResultado = tamX+filterLength;


    //vector señal
    //int *A = malloc( tamX * sizeof(int) );

    double A[] = {33, 36, 27, 15, 43, 35, 36, 42, 49, 21, 12, 27, 40, 9, 13, 26, 40, 26, 22, 36, 11, 18, 17, 29, 32, 30, 12, 23, 17, 35, 29, 2, 22, 8, 19, 17, 43, 6, 11, 42, 29, 23, 21, 19, 34, 37, 48, 24, 15, 20, 13, 26, 41, 30, 6, 23, 12, 20, 46, 31, 5, 25, 34, 27, 36, 5, 46, 29, 13, 7, 24, 45, 32, 45, 14, 17, 34, 14, 43, 0, 37, 8, 26, 28, 38, 34, 3, 1, 4, 49, 32, 10, 26, 18, 39, 12, 26, 36, 44, 39, 45, 20, 34, 28, 17, 1, 47, 2, 17, 42, 2, 6, 1, 30, 36, 41, 15, 39, 44, 19, 40, 29, 31, 17, 47, 21, 31, 25, 9, 27, 17, 6, 47, 3, 36, 15, 6, 33, 19, 24, 28, 21, 32, 29, 3, 19, 20, 18, 8, 15, 40, 49, 46, 23, 18, 45, 46, 1, 21, 5, 29, 38, 14, 28, 41, 0, 43, 0, 34, 14, 24, 14, 37, 6, 43, 41, 27, 15, 9, 36, 32, 1, 37, 28, 25, 7, 24, 21, 8, 45, 29, 37, 35, 43, 18, 28, 43, 11, 28, 29, 26, 4, 43, 13, 13, 38, 6, 40, 4, 18, 28, 38, 19, 17, 17, 46, 24, 43, 20, 33, 40, 49, 22, 25, 44, 40, 5, 39, 4, 36, 19, 32, 42, 14, 47, 7, 5, 4, 48, 11, 22, 28, 49, 43, 46, 18, 40, 22, 11, 10, 5, 1, 11, 30, 28, 5, 20, 36, 44, 26, 22, 15, 8, 16, 32, 8, 24, 37, 12, 24, 0, 36, 2, 49, 29, 0, 18, 21, 23, 31, 31, 30, 33, 44, 10, 13, 49, 31, 49, 46, 9, 23, 13, 18, 40, 45, 26, 16, 34, 40, 40, 34, 26, 42, 36, 7, 45, 6, 29, 18, 37, 12, 48, 22, 9, 9, 36, 10, 42, 37, 6, 1, 13, 22, 21, 5, 19, 49, 21, 4, 39, 11, 40, 17, 5, 28, 27, 0, 34, 8, 20, 24, 22, 19, 46, 31, 30, 34, 42, 22, 22, 0, 25, 35, 22, 49, 40, 42, 48, 13, 48, 40, 24, 40, 9, 31, 19, 36, 32, 5, 44, 4, 29, 19, 23, 26, 0, 5, 10, 42, 29, 34, 43, 5, 21, 17, 4, 13, 11, 4, 26, 9, 44, 2, 2, 6, 34, 21, 42, 18, 28, 39, 22, 8, 8, 48, 36, 8, 3, 48, 3, 33, 33, 48, 40, 4, 17, 46, 18, 29, 0, 46, 38, 47, 49, 40, 3, 33, 13, 47, 3, 42, 36, 25, 2, 46, 25, 38, 7, 29, 36, 10, 14, 21, 10, 4, 28, 27, 0, 48, 6, 2, 44, 47, 49, 43, 39, 2, 28, 3, 0, 31, 47, 38, 9, 1, 35, 34, 39, 42, 15, 27, 4, 29, 49, 14, 35, 29, 43, 35, 27, 0, 38, 21, 49, 39, 17, 38, 42, 45, 43, 44, 29, 40, 32, 40, 41, 19, 26, 32, 11, 42, 10, 17, 23, 11, 31, 9, 40, 25, 46, 17, 27, 34, 40, 26, 24, 7, 14, 18, 5, 8, 12, 36, 0, 46, 26, 44, 16, 2, 28, 29, 46, 40, 47, 20, 1, 30, 31, 43, 7, 27, 12, 36, 14, 5, 12, 40, 12, 29, 10, 19, 39, 24, 5, 41, 20, 33, 37, 38, 38, 16, 20, 34, 6, 17, 6, 10, 49, 37, 5, 9, 17, 18, 45, 33, 23, 8, 23, 37, 39, 33, 7, 28, 7, 14, 21, 29, 0, 9, 18, 38, 25, 38, 24, 33, 7, 31, 43, 8, 20, 49, 17, 39, 19, 13, 22, 44, 23, 47, 31, 12, 32, 40, 42, 41, 7, 15, 21, 7, 24, 41, 47, 1, 31, 21, 37, 40, 4, 30, 48, 25, 31, 16, 16, 2, 31, 39, 46, 4, 38, 30, 18, 21, 20, 12, 12, 29, 27, 35, 36, 4, 26, 33, 7, 9, 7, 44, 49, 11, 27, 0, 36, 10, 18, 5, 13, 49, 44, 11, 5, 34, 41, 25, 5, 14, 39, 18, 43, 18, 5, 32, 22, 32, 17, 30, 43, 24, 26, 43, 36, 3, 43, 24, 14, 13, 29, 27, 12, 25, 38, 19, 10, 32, 44, 17, 46, 35, 37, 41, 3, 43, 23, 28, 25, 41, 10, 18, 17, 36, 13, 5, 40, 8, 30, 4, 21, 11, 33, 35, 39, 23, 4, 1, 5, 0, 18, 3, 35, 6, 45, 39, 49, 20, 17, 26, 13, 27, 46, 31, 15, 10, 36, 5, 20, 18, 11, 42, 32, 46, 29, 21, 20, 34, 22, 27, 34, 40, 33, 22, 48, 30, 13, 47, 0, 30, 23, 14, 9, 22, 47, 24, 32, 35, 32, 4, 4, 43, 48, 36, 40, 28, 9, 12, 12, 33, 41, 48, 23, 24, 22, 22, 4, 35, 21, 7, 15, 47, 21, 26, 19, 18, 1, 3, 3, 33, 7, 9, 28, 6, 47, 20, 34, 8, 34, 48, 41, 26, 48, 15, 2, 21, 39, 9, 6, 10, 16, 24, 9, 39, 0, 28, 9, 3, 31, 14, 38, 39, 26, 17, 47, 23, 37, 31, 32, 22, 31, 25, 0, 29, 40, 4, 0, 31, 13, 7, 44, 31, 31, 3, 20, 33, 32, 31, 37, 15, 46, 25, 4, 22, 42, 1, 47, 32, 34, 31, 6, 15, 7, 8, 45, 49, 12, 47, 33, 26, 4, 27, 9, 37, 32, 32, 21, 16, 13, 10, 32, 11, 35, 36, 35, 30, 40, 33, 14, 26, 16, 20, 42, 25, 28, 39, 25, 40, 36, 10, 18, 43, 37, 28, 32, 21, 10, 5, 38, 25, 15, 20, 37, 3, 8, 22, 33, 0, 7, 47, 27, 26, 19, 21, 1, 49, 10, 28, 39, 48, 38, 10, 43, 27, 40, 26, 49, 2, 31, 37, 27, 49, 7, 16, 2, 17, 41, 35, 18, 48, 34, 45, 26, 5, 16, 28, 4, 28, 8, 43, 28, 47, 5, 22, 24, 45, 0, 25, 47, 33, 12, 27, 32, 21 };


    //vector resultado convolucion normal
    double *B = malloc( tamResultado*sizeof(double) );

    //tamaño de bloque
    int block_size = 512;

    //tamaño buffer local a la siguiente potencia de 2
    int l_localBuffer = nextpw2(block_size+filterLength-1);
    //printf("size = %d\n",l_localBuffer);

    //vector buffer local
    double* localBuffer = (double *) calloc(l_localBuffer, sizeof(double));

    //vector resultado convolucion ovelap-add + un poco más para guardar la ultima parte del vector que falta
    double *C = (double *) calloc(tamResultado + block_size, sizeof(double));

    //vector de ultima pasada con valores 0
    double * lastPass = (double *) calloc(block_size, sizeof(double));

    /* generar datos aleatorios para el vector A*/
 /*   printf("[");
    for( j=0; j<tamX; j++ ) {
        A[j] = ((int) rand() % 50);
        printf("%d, ",A[j]);
    }
    printf("]\n");
*/

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    tic = clock();
    convNormalIndex(A,0,tamX,H1,filterLength,B);
    toc = clock();
    printf("Elapsed: %f miliseconds\n", (double)(toc - tic)*1000 / CLOCKS_PER_SEC);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    tic = clock();
    //#pragma omp parallel for default(none) shared(A,D,j,z,block_size,localBuffer,l_localBuffer,H_1,filterLength,tamX)
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
    printf("Elapsed: %f miliseconds for the imput parallel Overlap-ADD\n", (double)(toc - tic)*1000 / CLOCKS_PER_SEC);

    j = 0;
    while(localBuffer[j] != 0){
        C[last_index+block_size+j]= localBuffer[j];
        localBuffer[j] = 0;
        j++;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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