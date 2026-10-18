#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){

    char* filename;
    int dim = 512;
    int div;


    if(argc < 3){
        printf("Uso <fichero> <div>\n");
        return -1;
    }

    filename = argv[1];
    div = atoi(argv[2]);

    unsigned char **MatTotal = (unsigned char**)malloc(dim*sizeof(unsigned char*));
    for(int i = 0 ; i < dim ; i++){
        MatTotal[i] = (unsigned char*)malloc(dim*sizeof(unsigned char));
    }

    FILE * f = fopen(filename, "rb");

    for(int i = 0 ; i < dim ; i++){
        fread(MatTotal[i], sizeof(unsigned char), dim, f);
    }

    fclose(f);


    int filas_base = dim / div;
    int resto = dim % div;

    unsigned char ***submatrices = (unsigned char***)malloc(div*sizeof(unsigned char **)); //[div][filas][columnas]

    int filaoriginal = 0;

    for(int s = 0 ; s < div; s++){
        int filas = filas_base;
        if(s<resto){
            filas++;
        }

        submatrices[s] = (unsigned char**)malloc(filas*sizeof(unsigned char*));
        for(int i = 0 ; i < filas ; i++){
            submatrices[s][i] = (unsigned char*)malloc(dim*sizeof(unsigned char));

            memcpy(submatrices[s][i], MatTotal[filaoriginal], dim*sizeof(unsigned char));
            filaoriginal++;
        }

    }

    for(int i = 0 ; i < dim ; i++) free(MatTotal[i]);
    free(MatTotal);

    unsigned char *MaxFila = (unsigned char*)malloc(dim*sizeof(unsigned char));

    filaoriginal = 0;

    for(int s = 0 ; s < div ; s++){
    
        int filas = filas_base;
        if(s<resto){
            filas++;
        }

        for(int i = 0 ; i < filas ; i++){

            unsigned char maximo = submatrices[s][i][0];

            for(int j = 0 ; j < dim ; j++){
                if(submatrices[s][i][j] > maximo) maximo = submatrices[s][i][j];
            }

            MaxFila[filaoriginal] = maximo;

            printf("Fila %d | Submatriz %d | Fila interna %d | Maximo %u \n", filaoriginal, (s+1), i, (unsigned int)MaxFila[filaoriginal]);
            filaoriginal++;

        }
    }

    for(int s = 0 ; s < div ; s++){
        int filas = filas_base;
        if(s<resto){
            filas++;
        }

        for(int i = 0 ; i < filas ; i++){
            free(submatrices[s][i]);
        }
        free(submatrices[s]);
    }

    free(submatrices);
    free(MaxFila);
    
    return 0;
}