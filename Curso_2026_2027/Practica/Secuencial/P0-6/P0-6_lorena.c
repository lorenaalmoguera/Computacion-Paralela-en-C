#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]){
    char* filename;
    int dim = 80; // uint8_t ocupa un byte: 80 bytes son 80 elementos.
    uint8_t *VectIncrementando;
    uint8_t *VectDecrementando;

    int iguales = 0;

    if(argc < 2){
        printf("Uso: %s <fichero>\n", argv[0]);
        return -1;
    }

    filename = argv[1];

    VectIncrementando = (uint8_t*)malloc(dim*sizeof(uint8_t));
    VectDecrementando = (uint8_t*)malloc(dim*sizeof(uint8_t));

    FILE *f = fopen(filename, "rb");

    for(int i = 0 ; i < dim ; i++){
        fread(VectIncrementando+i, sizeof(uint8_t), 1, f);
    }

    fclose(f);


    memcpy(VectDecrementando, VectIncrementando, dim*sizeof(uint8_t));
    /*
    for(int i = 0 ; i < dim ; i++){
        *(VectDecrementado + i) = *(VectIncrementado + i);
    }
    
    */

    for(int i = 3 ; i < dim ; i++){
        if(VectIncrementando[i] > VectIncrementando[i-1] + VectIncrementando[i-2] - VectIncrementando[i-3]){
            VectIncrementando[i] = (uint8_t)(VectIncrementando[i-1] + VectIncrementando[i-2] - VectIncrementando[i-3]);
        }else{
            VectIncrementando[i] = VectIncrementando[i] / 2;
        }
    }

    for(int i = dim-1 ; i > 2 ; i--){
        if(VectDecrementando[i] > VectDecrementando[i-1] + VectDecrementando[i-2] - VectDecrementando[i-3]){
            VectDecrementando[i] = (uint8_t)(VectDecrementando[i-1]+VectDecrementando[i-2]-VectDecrementando[i-3]);
        }else{
            VectDecrementando[i] = VectDecrementando[i] / 2;
        }
    }

    for(int i = 0 ; i < dim ; i++){
        if(VectDecrementando[i] == VectIncrementando[i]) iguales++;
    }

    printf("Elementos no procesados de VectIncrementando: indices 0, 1 y 2\n");
    printf("Elementos no procesados de VectDecrementando: indices 0, 1 y 2\n");
    printf("Elementos que coinciden en ambos vectores: %d de %d\n", iguales, dim);
    printf("HPC: se pueden ejecutar en paralelo los recorridos de ambos vectores, "
           "porque son independientes tras la copia. Dentro de cada recorrido "
           "hay dependencias entre iteraciones, asi que no se deben repartir "
           "los indices directamente sin tenerlas en cuenta.\n");

    free(VectIncrementando);
    free(VectDecrementando);

    return 0;
}
