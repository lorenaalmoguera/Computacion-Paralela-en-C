#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int dimension(FILE * archivo){

    fseek(archivo, 0, SEEK_END);
    long tam = ftell(archivo);
    rewind(archivo);

    int total_bytes = tam / sizeof(double);
    int dim = (int)sqrt(total_bytes);
    if(dim * dim != total_bytes) return -1;

    return dim;
}


int main(int argc, char * argv[]){

    char * filename;
    filename = argv[1];

    FILE * f = fopen(filename, "rb");

    int dim = dimension(f);

    int filas = dim-10;
    int columnas = dim-7;

    double** m; // 150x150
    double** Mat_Red; // -10 filas y -7 columnas
    
    // memoria matriz in reducir
    m = (double**)malloc(dim*sizeof(double*));
    for(int i = 0 ; i < dim ; i++){
        m[i] = (double*)malloc(dim*sizeof(double));
    }

    Mat_Red = (double**)malloc(filas*sizeof(double*));
    for(int i = 0 ; i < filas ; i++){
        Mat_Red[i] = (double*)malloc(columnas*sizeof(double));
    }

    // lectura fichero

    for(int i = 0 ; i < dim ; i++) fread(m[i], sizeof(double), dim, f);

    // guardado en matriz reducida utilizando memcpy

    int pos_fila = 0;


    for(int i = 0 ; i < dim ; i++){
        if(i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 7 || i == 9 || i == dim-1 || i == dim-2 || i == dim-3) continue;
        
        int pos_columna = 0;
        for(int j = 0 ; j < dim ; j++){

            if(j == 0 || j == 3 || j == 4 || j == 6 || j == dim-1 || j == dim-2 || j == dim-3) continue;
            // memcpy -> 1. a donde copiar, 2. de donde copiar, 3. tam tipo dato
            memcpy(&Mat_Red[pos_fila][pos_columna],&m[i][j], sizeof(double));
            pos_columna++;
        }
        pos_fila++;
    }

    fclose(f);
    
    FILE * f2 = fopen("matrizreducida1.bin", "wb"); //wb = write binary

    for(int i = 0 ; i < filas ; i++){
        fwrite(Mat_Red[i], sizeof(double), columnas, f2);
    }

    fclose(f2);


    // fin trabajo matrices

    double * vector = (double*)malloc(filas*columnas*sizeof(double));

    int pos = 0;

    for(int i = 0 ; i < dim ; i++){
        if(i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 7 || i == 9 || i == dim-1 || i == dim-2 || i == dim-3) continue;
        
        for(int j = 0 ; j < dim ; j++){
            if(j == 0 || j == 3 || j == 4 || j == 6 || j == dim-1 || j == dim-2 || j == dim-3) continue;

            memcpy(&vector[pos], &m[i][j], sizeof(double));
            pos++;
        }
    }


    FILE * f3 = fopen("vectorreducido.bin", "wb");
    fwrite(vector, sizeof(double), filas * columnas, f3);

    fclose(f3);

    for(int i = 0 ; i < dim ; i++) free(m[i]);
    free(m);
    
    for(int i = 0 ; i < filas ; i++) free(Mat_Red[i]);
    free(Mat_Red);
    free(vector);

    
    return 0;
}