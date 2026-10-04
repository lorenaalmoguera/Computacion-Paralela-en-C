#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
Se dispone de un fichero binario que contiene dos vectores de datos almacenados
consecutivamente. El tamaño de ese fichero es de 400 bytes
El programa deberá trabajar con dos vectores, V1 y V2 que tendrán el mismo tamaño. 

La cantidad de datos de tipo entero que se deben leer se indicará mediante un argumento
en la línea de ejecución del programa. Se sabe que dicha cantidad será como máximo de
50 enteros. 
*/

int main(int argc, char* argv[]){

    char * filename;
    int tam;
    int *v1;
    int *v2;  

    int max1 = -1000000000000000000;
    int max2 = -1000000000000000000;
    int maxglobal = 0;

    if (argc < 3) {
        printf("Uso: %s <fichero> <tam>\n", argv[0]);
    }

    filename = argv[1];
    tam = atoi(argv[2]);

    if(tam < 1){
        printf("La cantidad de datos de tipo entero a leer debe ser mayor a 0.");
        return -1;
    }else if(tam > 50){
        printf("La cantidad de datos de tipo entero a leer no debe ser mayor a 50.");
        return -1;
    }

    // reserva memoria

    v1 = (int*)malloc(tam*sizeof(int));
    v2 = (int*)malloc(tam*sizeof(int));

    FILE *f = fopen(filename, "rb");

    for(int i = 0 ; i < tam ; i++){
        fread(v1+i, sizeof(int), 1, f);
    }

    for(int i = 0 ; i < tam ; i++){
        fread(v2+i, sizeof(int), 1, f);
    }

    fclose(f);

    for(int i = 0 ; i < tam ; i++){
        if(v1[i] > max1){
            max1 = v1[i];
        }
        if(v2[i] > max2){
            max2 = v2[i];
        }
    }

    if(max1>max2){
        maxglobal = max1;
    }else{
        maxglobal = max2;
    }

    printf("El numero maximo es %d", maxglobal);
    free(v1);
    free(v2);

    return 0;
}
