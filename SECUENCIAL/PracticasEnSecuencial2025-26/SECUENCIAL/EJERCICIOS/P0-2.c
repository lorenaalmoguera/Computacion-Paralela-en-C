#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>


int main(int argc, char * argv[]){

    char * filename;
    double escalar;
    double* vector;
    double **matriz;
    if(argc < 2) return 1;

    filename = argv[1];

    FILE * f = fopen(filename, "rb");

    for (int i = 0 ; i < 10 ; i++){
        fread(&escalar, sizeof(double), 1, f);  // al ser un bucle 10 veces, serán 10 lecturas
        printf("Escalar: %.10lf \n", escalar);
    }

    rewind(f);
    vector = (double*)malloc(10*sizeof(double));

    fread(vector, sizeof(double), 10, f); // 1 lectura

    for(int i = 0 ; i < 10 ; i++){
        printf("V[%d] = %.10lf",i, vector[i]);
    }

    rewind(f);
    // el enunciado obliga a que no sea un bloque continuo... FILA A FILA
    matriz = (double**)malloc(5*sizeof(double*));
    for(int i = 0 ; i < 5 ; i++){
        matriz[i] = (double*)malloc(2*sizeof(double));
    }


    for(int i = 0 ; i < 5 ; i++){
        fread(matriz[i], sizeof(double), 2, f);
    }


    for(int i = 0 ; i < 5 ; i++){
        for(int j = 0 ; j < 2 ; j++){
            printf("M[%d , %d] = %.10lf", i, j, matriz[i][j]);
        }
    }


    fclose(f);

    

    return 0;

}