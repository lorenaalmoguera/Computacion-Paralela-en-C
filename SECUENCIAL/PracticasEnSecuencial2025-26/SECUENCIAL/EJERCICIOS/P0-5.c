#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]){

    char* nombrefichero;
    int dim;
    unsigned char **m;
    double *v_fil, *v_col;

    dim = atoi(argv[1]);
    nombrefichero = argv[2];

    m = (unsigned char**)malloc(dim * sizeof(unsigned char*));
    for(int i = 0; i < dim; i++){
        m[i] = (unsigned char*)malloc(dim * sizeof(unsigned char));
    }

    v_fil = (double*)malloc(dim * sizeof(double));
    v_col = (double*)malloc((dim/2) * sizeof(double));

    FILE *f = fopen(nombrefichero, "rb");
    if(f == NULL){
        printf("Error: no se puede abrir el archivo %s\n", nombrefichero);
        return 1;
    }

    for(int i = 0; i < dim; i++){
        fread(m[i], sizeof(unsigned char), dim, f);
    }

    fclose(f);

    // calcular media de cada fila
    double aux;
    for(int i = 0; i < dim; i++){
        aux = 0;
        for(int j = 0; j < dim; j++){
            aux += m[i][j];
        }
        v_fil[i] = aux / dim;
    }

    // calcular media de columnas pares
    for(int j = 0; j < dim; j += 2){
        aux = 0;
        for(int i = 0; i < dim; i++){
            aux += m[i][j];
        }
        v_col[j/2] = aux / dim;
    }

    // calcular desviacion tipica de cada fila
    double des_tipica;
    double *desv_tipica = (double*)malloc(dim * sizeof(double));

    for(int i = 0; i < dim; i++){
        des_tipica = 0;
        for(int j = 0; j < dim; j++){
            des_tipica += (m[i][j] - v_fil[i]) * (m[i][j] - v_fil[i]);
        }
        desv_tipica[i] = sqrt(des_tipica / dim);
    }

    printf("Desviacion estandar de las filas...\n");

    printf("0: %.10f\n", desv_tipica[0]);
    printf("1: %.10f\n", desv_tipica[1]);
    printf("2: %.10f\n", desv_tipica[2]);
    printf("100: %.10f\n", desv_tipica[100]);
    printf("N-3: %.10f\n", desv_tipica[dim-3]);
    printf("N-2: %.10f\n", desv_tipica[dim-2]);

    // calcular maximo y minimo global de la media de todas las filas
    double max_filas = v_fil[0];
    double min_filas = v_fil[0];

    for(int i = 1; i < dim; i++){
        if(v_fil[i] < min_filas){
            min_filas = v_fil[i];
        }

        if(v_fil[i] > max_filas){
            max_filas = v_fil[i];
        }
    }

    printf("Maximo filas: %.10f\n", max_filas);
    printf("Minimo filas: %.10f\n", min_filas);

    // calcular maximo y minimo global de la media de las columnas pares
    double max_columnas = v_col[0];
    double min_columnas = v_col[0];

    for(int i = 1; i < dim/2; i++){
        if(v_col[i] < min_columnas){
            min_columnas = v_col[i];
        }

        if(v_col[i] > max_columnas){
            max_columnas = v_col[i];
        }
    }

    printf("Maximo columnas pares: %.10f\n", max_columnas);
    printf("Minimo columnas pares: %.10f\n", min_columnas);

    for(int i = 0; i < dim; i++){
        free(m[i]);
    }

    free(m);
    free(v_fil);
    free(v_col);
    free(desv_tipica);

    return 0;
}