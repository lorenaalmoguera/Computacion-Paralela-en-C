#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]){

    char* nombrefichero;
    int dim;

    // PARTE 0

    // OBTENER PARÁMETROS
    dim = atoi(argv[1]);
    nombrefichero = argv[2];

    // REESERVAR MEMORIA

    unsigned char *v = (unsigned char*)malloc(dim*dim*sizeof(unsigned char));

    // para dim filas -> 150

    double *v_filas = (double*)malloc(dim*sizeof(double));

    // para dim / 2 columnas -> 75

    double *v_columnas = (double*)malloc((dim/2)*sizeof(double));

    // desv tipica

    double *v_des_tipica = (double*)malloc((6)*sizeof(double));

   // LEER FICHERO 
    FILE *f = fopen(nombrefichero, "rb");
    for(int i = 0 ; i < dim ; i++) fread(v, sizeof(unsigned char), dim*dim, f);
    fclose(f);


    // PARTE 1

    // calcular y almacenar la media de cada fila -> vector

    double datos;
    for(int i = 0 ; i < dim ; i++){
        datos = 0;
        for(int j = 0 ; j < dim ; j++){
            datos += v[i*dim+j];
        }
        v_filas[i] = datos / dim; // calculo media
        
    }

    // calcular y almacenar la media de las columnas pares ->

    for(int j = 0 ; j < dim ; j+= 2){
        datos = 0;
        for(int i = 0 ; i < dim ; i++){
            datos += v[i*dim+j];
        }
        v_columnas[j/2] = datos / dim; // calculo media
    }

    // PARTE 2

    // calcular y mostrar desviacion típica : 0 1 2 100 N-3 N-2

    double des;
    int pos = 0;
    for(int i = 0 ; i < dim ; i++){
        if( i != 0 && i != 1 && i != 2 && i != 100 && i != dim-3 && i != dim-2) continue;
        des = 0;
        for(int j = 0 ; j < dim ; j++){
            des += (v[i*dim+j] - v_filas[i]) * (v[i*dim+j] - v_filas[i]);
        }
        v_des_tipica[pos] = sqrt(des/dim);
        pos++;
    }

    // impresion de los datos

    for(int i = 0 ; i < 6 ; i++){
        printf("Desv tipica: %.10f \n", v_des_tipica[i]);
        // 0, 1, 2, 100, N-3, N-2
    }

    // PARTE 3
    // maximo y minimo de todas las filas || maximo y minimo columnas pares

    double max_filas, max_columnas, min_filas, min_columnas;


    min_filas = v_filas[0];
    max_filas = v_filas[0];

    for(int i = 1 ; i < dim ; i++){
        
        //min
        if(v_filas[i] < min_filas){
            min_filas = v_filas[i];
        }

        //max
        if(v_filas[i] > max_filas){
            max_filas = v_filas[i];
        }
    }

    printf("Media minima filas: %.10f\n", min_filas);
    printf("Media maxima filas: %.10f\n", max_filas);

    min_columnas = v_columnas[0];
    max_columnas = v_columnas[0];
    
    for (int i = 1 ; i < dim/2 ; i++){
        
        //min
        if(v_columnas[i] < min_columnas){
            min_columnas = v_columnas[i];
        }

        //max
        if(v_columnas[i] > max_columnas){
            max_columnas = v_columnas[i];
        }
    }
    

    printf("Media minima columnas: %.10f\n", min_columnas);
    printf("Media maxima columnas: %.10f\n", max_columnas);

    free(v);
    free(v_columnas);
    free(v_des_tipica);
    free(v_filas);

    return 0;
}