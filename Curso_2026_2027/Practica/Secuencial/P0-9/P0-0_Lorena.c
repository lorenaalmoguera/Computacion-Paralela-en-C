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

    // El enunciado permite dividir la matriz entre 2 y 5 submatrices.
    // Esta comprobación también evita dividir entre cero más abajo.
    if(div < 2 || div > 5){
        printf("El numero de submatrices debe estar entre 2 y 5.\n");
        return -1;
    }

    FILE *f = fopen(filename, "rb");
    if(f == NULL){
        printf("Error: no se pudo abrir el fichero '%s'\n", filename);
        return -1;
    }

    // MatTotal se reserva por filas: cada fila contiene 512 unsigned char.
    unsigned char **MatTotal = (unsigned char**)malloc(dim * sizeof(unsigned char*));
    for(int i = 0; i < dim; i++){
        MatTotal[i] = (unsigned char*)malloc(dim * sizeof(unsigned char));
    }

    // El fichero almacena las filas consecutivamente.
    for(int i = 0; i < dim; i++){
        fread(MatTotal[i], sizeof(unsigned char), dim, f);
    }
    fclose(f);

    int columnas_base = dim / div;
    int resto = dim % div;

    // submatrices[s] apunta a 512 filas; cada fila tiene el ancho
    // correspondiente al bloque de columnas de esa submatriz.
    unsigned char ***submatrices =
        (unsigned char***)malloc(div * sizeof(unsigned char**));

    int inicio_columna = 0;

    for(int s = 0; s < div; s++){
        int columnas = columnas_base;
        if(s < resto){
            columnas++;
        }

        submatrices[s] = (unsigned char**)malloc(dim * sizeof(unsigned char*));

        for(int i = 0; i < dim; i++){
            submatrices[s][i] =
                (unsigned char*)malloc(columnas * sizeof(unsigned char));

            // En cada fila, las columnas del bloque son contiguas.
            // inicio_columna señala la primera columna que se copia.
            memcpy(submatrices[s][i],
                   MatTotal[i] + inicio_columna,
                   columnas * sizeof(unsigned char));
        }

        // El siguiente bloque comienza después de las columnas copiadas.
        inicio_columna += columnas;
    }

    // Desde aquí en adelante los datos se consultan solo en submatrices.
    for(int i = 0; i < dim; i++){
        free(MatTotal[i]);
    }
    free(MatTotal);

    unsigned char *MaxColumna =
        (unsigned char*)malloc(dim * sizeof(unsigned char));

    inicio_columna = 0;

    for(int s = 0; s < div; s++){
        int columnas = columnas_base;
        if(s < resto){
            columnas++;
        }

        // j_local es la posición dentro de esta submatriz.
        for(int j_local = 0; j_local < columnas; j_local++){
            unsigned char maximo = submatrices[s][0][j_local];

            // La columna está repartida entre las 512 filas de esta submatriz.
            for(int i = 1; i < dim; i++){
                if(submatrices[s][i][j_local] > maximo){
                    maximo = submatrices[s][i][j_local];
                }
            }

            // Convertimos el índice local en el índice de la matriz original.
            int j_global = inicio_columna + j_local;
            MaxColumna[j_global] = maximo;

            printf("Columna %d | Maximo %u\n",
                   j_global, (unsigned int)MaxColumna[j_global]);
        }

        inicio_columna += columnas;
    }

    // Se libera cada fila, luego su tabla de punteros y, finalmente, la tabla general.
    for(int s = 0; s < div; s++){
        for(int i = 0; i < dim; i++){
            free(submatrices[s][i]);
        }
        free(submatrices[s]);
    }

    free(submatrices);
    free(MaxColumna);

    return 0;
}
