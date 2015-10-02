#include <stddef.h>
#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//ceil
//#include <math.h>
//#include <string.h>
#include "../import/muestra.h"


int main(void)
{
    size_t j;
    printf("Vector\n");
    //int j;
    for( j=0; j<signalLength; j++ ) {
        printf("%f\n ",soundRandom[j]);
    }


    return 0;
}
