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
    int size;
    int *v1;
    int *v2;  
    
    if (argc < 3) {
        printf("Uso: %s <fichero> <cantidad>\n", argv[0]);
    }

    filename = argv[1];
    size = atoi(argv[2]);

    if(size < 2){
        printf("La cantidad de datos de tipo entero a leer debe ser mayor a 1.");
        return -1;
    }else if(size > 50){
        printf("La cantidad de datos de tipo entero a leer no debe ser mayor a 50.");
        return -1;
    }

    // 200 bytes / 4 bytes por entero = 50 enteros por vector

    v1 = (int*)malloc(size*sizeof(int));
    v2 = (int*)malloc(size*sizeof(int));

    FILE * f = fopen(filename, "rb");
    if (f == NULL) {
        fprintf(stderr, "Error: no se pudo abrir '%s': ", filename);
        perror(NULL);
        return EXIT_FAILURE;
    }

    for(int i = 0 ; i < size ; i++){
        fread(v1 + i, sizeof(int), 1, f);
    }

    for(int i = 0 ; i < size ; i++){
        fread(v2 + i, sizeof(int), 1, f);
    }

    fclose(f);

    for(int i = 0 ; i < size ; i++){
        printf("V1[%d] = %d\n", i, *(v1+i));
    }
    for(int i = 0 ; i < size ; i++){
        printf("V2[%d] = %d\n", i, *(v2+i));
    }

    free(v1);
    free(v2);

    return 0;
}
