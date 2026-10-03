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

    // reparto de trabajo
        // 200 bytes / 4 bytes por entero = 50 enteros por vector
    FILE * f = fopen(filename, "rb");

    for(int i = 0 ; i < tam ; i++){
        fread(v1+i,sizeof(int), 1, f);
    }

    for(int i = 0 ; i < tam ; i++){
        fread(v2+i,sizeof(int), 1, f);
    }

    fclose(f);

    // resultado
    for(int i = 0 ; i < tam ; i++){
        printf("v1[%d] = %d\n", i, *(v1+i));
    }
    
    for(int i = 0 ; i < tam ; i++){
        printf("v1[%d] = %d \n", (i + tam), *(v2+i));
    }


    free(v1);
    free(v2);

    return 0;
}
