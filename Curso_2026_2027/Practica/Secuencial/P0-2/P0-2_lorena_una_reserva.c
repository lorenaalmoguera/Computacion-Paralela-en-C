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
    int *datos;

    int max_v1 = -999999;
    int max_v2 = -999999;
    int maxglobal = 0;
    
    if (argc < 3) {
        printf("Uso: %s <fichero> <cantidad>\n", argv[0]);
        return -1;
    }

    filename = argv[1];
    tam = atoi(argv[2]);

    if(tam < 1){
        printf("La cantidad de datos de tipo entero a leer debe ser mayor a 0.");
        return -1;
    }else if(tam > 200){
        printf("La cantidad de datos de tipo entero a leer no debe ser mayor a 50.");
        return -1;
    }

    datos = (int*)malloc(tam*2*sizeof(int));
    v1 = datos;
    v2 = datos + tam;

    FILE * f = fopen(filename, "rb");

    for(int i = 0 ; i < tam ; i++){
        fread(v1+i,sizeof(int), 1, f);
    }
    
    for(int i = 0 ; i < tam ; i++){
        fread(v2+i,sizeof(int), 1, f);
    }

    fclose(f);

    // numero maximo

    for(int i = 0 ; i < tam ; i++){
        if(*(v1+i)>max_v1){
            max_v1 = *(v1+i);
        }
        if(*(v2+i)>max_v2){
            max_v2 = *(v2+i);
        }
    }

    if(max_v1 > max_v2){
        maxglobal = max_v1;
    }else{
        maxglobal = max_v2;
    }

    printf("el numero maximo del fichero es %d\n", maxglobal);

    // liberar memoria
    free(datos);

    return 0;
}
