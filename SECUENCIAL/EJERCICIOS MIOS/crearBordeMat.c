#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char * argv[]){

    int filas, columnas;
    char * filename;

    if(argc < 4) return 1;

    filas = atoi(argv[1]);
    columnas = atoi(argv[2]);
    filename = argv[3];

    filas += 2;
    columnas += 2;

    float **matriz = (float**)malloc(filas * sizeof(float*));
    
    for(int i = 0 ; i < filas ; i++){
        matriz[i] = (float*)malloc(columnas * sizeof(float));
    }


    for(int i = 0 ; i < filas ; i++){
        for(int j = 0 ; j < columnas ; j++){
            matriz[i][j] = 0;
        }
    }

    for(int i = 0 ;  i < filas ; i++){
        for(int j = 0 ; j < columnas ; j++){
            printf("M[ %8.2f ] ", matriz[i][j]);
        }
        printf("\n");
    }

    FILE * f = fopen(filename, "rb");

    for(int i = 1 ; i < filas-1 ; i++) fread(&matriz[i][1], sizeof(float), columnas - 2, f);

    fclose(f);

    printf("\n\nDATOS DEL FICHERO\n");

    for(int i = 0 ;  i < filas ; i++){
        for(int j = 0 ; j < columnas ; j++){
            printf("M[ %8.2f ] ", matriz[i][j]);
        }
        printf("\n");
    }

        for(int i = 0 ; i < filas ; i++){
        free(matriz[i]);
    }


    free(matriz);

    return 0;

}