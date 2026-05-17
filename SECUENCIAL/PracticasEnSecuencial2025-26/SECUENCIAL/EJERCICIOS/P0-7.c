#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

int main(int argc, char* argv[]){

    char* nombrefichero;

    // PARTE 0

    // OBTENER PARÁMETROS
    nombrefichero = argv[1];

    // REESERVAR MEMORIA

    uint8_t *VectIncrementado = (uint8_t*)malloc(80*sizeof(uint8_t));

    uint8_t *VectDecrementado = (uint8_t*)malloc(80*sizeof(uint8_t));

   // LEER FICHERO en ambos vectores
    FILE *f = fopen(nombrefichero, "rb");
    fread(VectIncrementado, sizeof(uint8_t), 80, f);
    fread(VectDecrementado, sizeof(uint8_t), 80, f);  // O copiar
    fclose(f);

    int dim = 80;

    // copia de ddatos

    for(int i = 0 ; i < dim ; i++){
        *(VectIncrementado + i) = *(VectDecrementado + i);
    }

    // Bucle INCREMENTANDO (desde i=3 para evitar índices negativos)
    for(int i = 3 ; i < 80 ; i++){
        if(VectIncrementado[i] > VectIncrementado[i-1] + VectIncrementado[i-2] - VectIncrementado[i-3]){
            VectIncrementado[i] = (uint8_t)(VectIncrementado[i-1] + VectIncrementado[i-2] - VectIncrementado[i-3]);
        }else{
            VectIncrementado[i] = VectIncrementado[i] / 2;
        }
    }

    // Bucle DECREMENTANDO (desde i = dim hasta i >= 3)
    for(int i = dim ; i >= 3 ; i--){
        if(VectDecrementado[i] > VectDecrementado[i-1] + VectDecrementado[i-2] - VectDecrementado[i-3]){
            VectDecrementado[i] = (uint8_t)(VectDecrementado[i-1] + VectDecrementado[i-2] - VectDecrementado[i-3]);
        }else{
            VectDecrementado[i] = VectDecrementado[i] / 2;
        }
    }


    printf("Vector Incrementado.");
    printf("\n");
    printf("\n");
    for(int i = 0 ; i < dim ; i++){
        printf("%d", VectIncrementado[i]);
        printf("\n");
    }
    printf("\n");
    printf("Vector Decrementado");
    printf("\n");
    printf("\n");
    for(int i = 0 ; i < dim ; i++){
        printf("%d", VectDecrementado[i]);
        printf("\n");
    }

    free(VectDecrementado);
    free(VectIncrementado);
    return 0;
}