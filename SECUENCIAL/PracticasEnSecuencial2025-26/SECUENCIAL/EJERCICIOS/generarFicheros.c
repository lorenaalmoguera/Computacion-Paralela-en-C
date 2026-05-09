#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char * argv[]) {

    char* fichero;
    int filas, columnas;

    if(argc < 4) return 1;

    filas = atoi(argv[1]);
    columnas = atoi(argv[2]);
    fichero = argv[3];

    double **matriz = (double**)malloc(filas * sizeof(double*));
    
    for(int i = 0 ; i < filas ; i++) {
        matriz[i] = (double*)malloc(columnas * sizeof(double));
    }

    srand(time(NULL));

    for(int i = 0 ; i < filas ; i++) {
        for(int j = 0 ; j < columnas ; j++) {
            matriz[i][j] = (double)rand() / (double)RAND_MAX * 100.0;
        }
    }

    FILE *f = fopen(fichero, "wb");

    if(f == NULL) {
        printf("Error al abrir el fichero\n");
        return 1;
    }

    //fwrite(&filas, sizeof(int), 1, f);
    //fwrite(&columnas, sizeof(int), 1, f);

    for(int i = 0 ; i < filas ; i++) {
        fwrite(matriz[i], sizeof(double), columnas, f);
    }

    fclose(f);

    for(int i = 0 ; i < filas ; i++) {
        free(matriz[i]);
    }

    free(matriz);

    return 0;
}