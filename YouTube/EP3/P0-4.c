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

    // m[i][j] -> *(*(m+i)+j);
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

    /*for(int i = 0 ; i < dim ; i++){
        for(int j = 0 ; j < dim ; j++){
            printf("M[%d , %d] = %.8lf", i, j, m[i][j]);
        }
        printf("\n");
    }*/

    fclose(f);

    double **m_aux = (double**)malloc(filas * sizeof(double*));
    for(int i = 0 ; i < filas ; i++){
        m_aux[i] = (double*)malloc(columnas * sizeof(double));
    }

    int fila_aux = 0;

    for(int i = 0 ; i < dim ; i++){

        if(i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 7 || i == 9 ||
        i == dim-1 || i == dim-2 || i == dim-3) continue;

        int col_aux = 0;

        for(int j = 0 ; j < dim ; j++){

            if(j == 0 || j == 3 || j == 4 || j == 6 ||
            j == dim-1 || j == dim-2 || j == dim-3) continue;

            //memcpy(&m_aux[fila_aux][col_aux], &m[i][j], sizeof(double)); // void*, void*
            *(*(m_aux+fila_aux)+col_aux) = *(*(m+i)+j);
            col_aux++;
        }

        fila_aux++;
    }

    FILE * f2 = fopen("matrizreducida1.bin", "wb");

    for(int i = 0 ; i < filas ; i++){
        fwrite(m_aux[i], sizeof(double), columnas, f2);
    }

    fclose(f2);

    double *vector = (double*)malloc(filas*columnas*sizeof(double));

    int pos = 0;

    for(int i = 0 ; i < dim ; i++){
        if(i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 7 || i == 9 ||
        i == dim-1 || i == dim-2 || i == dim-3) continue;

        for(int j = 0 ; j < dim ; j++){

            if(j == 0 || j == 3 || j == 4 || j == 6 ||
            j == dim-1 || j == dim-2 || j == dim-3) continue;

            //memcpy(&vector[pos], &m[i][j], sizeof(double)); // void*, void*
            *(vector+pos) = *(*(m+i)+j);
            pos++;
        }
    }

    FILE * f3 = fopen("vectorreducido1.bin","wb");
    fwrite(vector, sizeof(double), filas * columnas, f3);


    fclose(f3);

    for(int i = 0; i < dim; i++) free(m[i]);
    free(m);

    for(int i = 0 ; i < filas ; i++) free(m_aux[i]);
    free(m_aux);

    free(vector);
    return 0;
}