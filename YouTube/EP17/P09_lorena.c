#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    char *filename;
    int dim = 512;
    int div;

    if(argc < 3){
        printf("Uso: %s <fichero> <div>\n", argv[0]);
        return -1;
    }

    filename = argv[1];
    div = atoi(argv[2]);

    if(div < 2 || div > 5){
        printf("El numero de submatrices debe estar entre 2 y 5.\n");
        return -1;
    }

    FILE *f = fopen(filename, "rb");

    unsigned char **MatTotal = (unsigned char**)malloc(dim*sizeof(unsigned char*));
    for(int i = 0 ; i < dim ; i++) MatTotal[i] = (unsigned char*)malloc(dim*sizeof(unsigned char));

    for(int i = 0; i < dim ; i++) fread(MatTotal[i], sizeof(unsigned char), dim, f);
    fclose(f);

    int columnas_base = dim / div;
    int resto = dim % div;

    unsigned char ***submatrices = (unsigned char***)malloc(div*sizeof(unsigned char**));

    int inicio_columna = 0;

    for(int s = 0 ; s < div ; s++){
        int columnas = columnas_base;
        if (s < resto){
            columnas++;
        }

        submatrices[s] = (unsigned char**)malloc(dim*sizeof(unsigned char*));

        for(int i = 0 ; i < dim ; i++){
            submatrices[s][i] = (unsigned char*)malloc(columnas*sizeof(unsigned char));

            memcpy(submatrices[s][i], MatTotal[i] + inicio_columna, columnas*sizeof(unsigned char));
        }

        inicio_columna += columnas;
    }

    for(int i = 0 ; i < dim ; i++){
        free(MatTotal[i]);
    }
    free(MatTotal);

    unsigned char *MaxColumna = (unsigned char*)malloc(dim*sizeof(unsigned char));


    inicio_columna = 0 ;

    for(int s = 0 ; s < div ; s++){
        int columnas = columnas_base;
        if (s < resto){
            columnas++;
        }


        for(int j_submatriz = 0 ; j_submatriz < columnas ; j_submatriz++){
            unsigned char maximo = submatrices[s][0][j_submatriz];
            for(int i = 1 ; i < dim ; i++){
                if(submatrices[s][i][j_submatriz] > maximo){
                    maximo = submatrices[s][i][j_submatriz];
                }
            }
            int j_mat_total = inicio_columna + j_submatriz;
            MaxColumna[j_mat_total] = maximo;

            printf("Columna %d | Maximo %u \n", j_mat_total, (unsigned int)MaxColumna[j_mat_total]);
        }
        inicio_columna = inicio_columna + columnas;
    }

    for(int s = 0 ; s < div ; s++){
        for(int i = 0 ; i <  dim ; i++){
            free(submatrices[s][i]);
        }
        free(submatrices[s]);
    }

    free(submatrices);
    free(MaxColumna);

    return 0;
}