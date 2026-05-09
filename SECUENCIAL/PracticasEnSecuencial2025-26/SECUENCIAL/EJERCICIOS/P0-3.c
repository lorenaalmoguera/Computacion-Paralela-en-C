#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int dimension(FILE* archivo){

    fseek(archivo, 0, SEEK_END);
    long tam = ftell(archivo);
    rewind(archivo);

    int total_bytes = tam / sizeof(double);
    int dim = (int)sqrt(total_bytes);
    if (dim * dim != total_bytes) return -1;

    return dim ;
}

int main(int argc, char* argv[]) {

    char* filename;

    int dim, filas, columnas;

    if(argc < 2) return 1;

    filename = argv[1];

    FILE *f = fopen(filename, "rb");

    dim = dimension(f);

    if(dim == -1) return 1;

    filas = dim;
    columnas = dim;

    filas = filas - 10;
    columnas = columnas - 7;

    double **m = (double**)malloc(dim * sizeof(double*));
    for(int i = 0 ; i < dim ; i++){
        m[i] = (double*)malloc(dim*sizeof(double));
    }


    for(int i = 0 ; i < dim ; i++) fread(m[i], sizeof(double), dim, f);

    for(int i = 0 ; i < dim ; i++){
        for(int j = 0 ; j < dim ; j++){
            printf("M[%d , %d] = %.8lf", i, j, m[i][j]);
        }
        printf("\n");
    }

    fclose(f);

    for(int i = 0; i < dim; i++) free(m[i]);
    free(m);

    return 0;
}