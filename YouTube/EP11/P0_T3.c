#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){

    char * filename;
    int tam = 100;
    int *v1;

    int max1 = -999999999;
    int max2 = -999999999;
    int maxglobal = 0;

    if (argc < 2) {
        printf("Uso: %s <fichero> \n", argv[0]);
        return 0;
    }

    filename = argv[1];

    // reserva memoria

    v1 = (int*)malloc(tam*sizeof(int));

    FILE *f = fopen(filename, "rb");

    for(int i = 0 ; i < tam ; i ++){
        fread(v1+i, sizeof(int), 1, f);
    }

    fclose(f);

    for(int i = 0; i < 4 ; i++){
        printf("v1[%d] = %d\n", i, *(v1+i));
    }


    /* *pintV1 apunta al primer entero de V1. */
    int *pintV1 = v1;
    *(pintV1 + 0) = 1;

    printf("*pintV1 = %d\n", *(pintV1+0));
    printf("*v1[0] = %d\n", *(v1+0));

    /* *pcharV1 apunta al primer byte de V1. */
    char *pcharV1 = (char *)v1;
    *(pcharV1 + 0) = 2;

    printf("*pcharV1 = %d\n", (int)*(pcharV1+0));
    printf("*v1[0] = %d\n", *(v1+0));

    /* *pdoubleV1 apuntar a los int mediante double (experimental). */
    double * pdoubleV1 = (double*)v1;
    *(pdoubleV1 + 0) = 3;
    printf("*pdoubleV1 = %f\n", *(pdoubleV1+0));
    printf("*v1[0] = %d\n", *(v1+0));

    free(v1);
    return 0;
}
