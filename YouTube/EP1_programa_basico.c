#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//LECTURA DE UNA MATRIZ 150x150 GUARDADA EN UN FICHERO
    
    // los parámetros
    // lectura de ficheros .bin -> fread(...)
    // malloc


int main(int argc, char * argv[]){
    int col, filas;
    double **m;
    char* nombrefichero;

    col = atoi(argv[1]);
    filas = atoi(argv[2]);
    nombrefichero = argv[3];


    m = (double**)malloc(filas * sizeof(double*));
    for (int i = 0 ; i < filas ; i++){
        m[i] = (double*)malloc(col * sizeof(double));
    }
    
    FILE * f = fopen(nombrefichero, "rb"); // read binary

    for(int i = 0 ; i < filas ; i++) fread(m[i], sizeof(double), col, f);

    fclose(f);


    // TRABAJO

        // vecinos
        // la media
        // desviacion típica
        // la norma

    //



    for(int i = 0 ; i < filas ; i++){
        for(int j = 0 ; j < col ; j++){
            printf("M[%d, %d] = %.2lf ; ", i, j, m[i][j]);
        }
        printf("\n");
    }

    for (int i = 0 ; i < filas ; i++){
        free(m[i]);
    }

    free(m);

    return 0;
 
}
