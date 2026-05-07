#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

// EL ENUNCIADO ES EL DE 0-1 PERO ESTÁ ADAPTADO A LAS MATRICES


int dimension(FILE* archivo){

    fseek(archivo, 0, SEEK_END);
    long tam = ftell(archivo);
    rewind(archivo);

    int total_bytes = tam / sizeof(double);
    int dim = (int)sqrt(total_bytes);
    if (dim * dim != total_bytes) return -1;

    return dim ;
}


int main(int argc, char * argv[]){

    int dim;
    float media = 0;
    char * filename;

    if(argc < 2) return 1;

    filename = argv[1];

    FILE * f = fopen(filename, "rb");

    dim = dimension(f);

    if(dim == -1) return 1;

    double **m = (double**)malloc(dim * sizeof(double*));
    
    for(int i = 0 ; i < dim ; i++){
        m[i] = (double*)malloc(dim*sizeof(double));
    }

    for (int i = 0 ; i < dim ; i++){
        fread(m[i], sizeof(double), dim, f);
    }

    fclose(f);

    FILE *f1 = fopen("fichtxt1.txt", "w");
    FILE *f2 = fopen("fichtxt2.txt", "w");

    if(f1 == NULL || f2 == NULL){
        printf("Error al crear los ficheros de salida\n");
        return 1;
    }

    for(int i = 0 ; i < dim ; i++){
        for(int j = 0 ; j < dim ; j++){

            fprintf(f1, "%.10lf", m[i][j]);

            if(!(i == dim - 1 && j == dim - 1)){
                fprintf(f1, ";");
            }
        }
    }
    
    fclose(f1);

    for(int i = 0 ; i < dim ; i++){
        for(int j = 0 ; j < dim ; j++){
            fprintf(f2, "%.15lf\n", m[i][j]);
        }
    }

    fclose(f2);
    

    return 0;

}