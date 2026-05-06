#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char * argv[]){

    int filas, columnas;
    float media = 0;
    char * filename;

    if(argc < 4) return 1;

    filas = atoi(argv[1]);
    columnas = atoi(argv[2]);
    filename = argv[3];

    float **matriz = (float**)malloc(filas * sizeof(float*));
    
    for(int i = 0 ; i < filas ; i++){
        matriz[i] = (float*)malloc(columnas * sizeof(float));
    }

    FILE * f = fopen(filename, "rb");

    for(int i = 0 ; i < filas ; i++) fread(matriz[i], sizeof(float), columnas, f);

    fclose(f);

    printf("\n\nDATOS DEL FICHERO\n");

    for(int i = 0 ;  i < filas ; i++){
        for(int j = 0 ; j < columnas ; j++){
            printf("M[ %8.2f ] ", matriz[i][j]);
            media += matriz[i][j];
        }
        printf("\n");
    }

    media = media / (filas*columnas);

    printf("La media global es %8.2f\n", media);

    for(int i = 0 ; i < filas ; i++){
        for(int j = 0 ; j < columnas ; j++){
            if(matriz[i][j] < media){
                matriz[i][j] = matriz[i][j] *(-1);
            }
        }
    }

    for(int i = 0 ;  i < filas ; i++){
        for(int j = 0 ; j < columnas ; j++){
            printf("M[ %8.2f ] ", matriz[i][j]);
            media += matriz[i][j];
        }
        printf("\n");
    }

    for(int i = 0 ; i < filas ; i++){
        free(matriz[i]);
    }

    free(matriz);

    return 0;

}