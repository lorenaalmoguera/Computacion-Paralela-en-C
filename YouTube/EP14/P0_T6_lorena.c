#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[]){

    char * filename;
    int dim = 80;
    uint8_t * VectIncrementado, *VectDecrementado;

    int iguales = 0;

    if (argc < 2){
        printf("Uso : <filename>\n");
        return -1;
    }

    filename = argv[1];

    VectIncrementado = (uint8_t*)malloc(dim*sizeof(uint8_t));
    VectDecrementado = (uint8_t*)malloc(dim*sizeof(uint8_t));

    FILE *f = fopen(filename, "rb");

    for(int i = 0 ; i < dim ; i++) fread(VectIncrementado+i, sizeof(uint8_t), 1, f);

    fclose(f);

    /*
    for(int i = 0 ; i < dim ; i++){
        *(VectDecrementado+i) = *(VectIncrementado+i);
    }
    */

    memcpy(VectDecrementado, VectIncrementado, dim*sizeof(uint8_t));

    /*

    Si v[i] es mayor que (v[i-1] + v[i-2] - v[i-3]), entonces:
        v[i] = (uint8_t)(v[i-1] + v[i-2] - v[i-3])
    De lo contrario:
        v[i] = v[i] / 2
    
    */
    for(int i = 3 ; i < dim ; i++){
        if(VectIncrementado[i] > (VectIncrementado[i-1]+VectIncrementado[i-2]-VectIncrementado[i-3])){
            VectIncrementado[i] = (uint8_t)(VectIncrementado[i-1]+VectIncrementado[i-2]-VectIncrementado[i-3]);
        }else{
            VectIncrementado[i] = VectIncrementado[i] / 2;
        }
    }

    for(int i = dim-1 ; i > 2 ; i--){
        if(VectDecrementado[i] > (VectDecrementado[i-1]+VectDecrementado[i-2]-VectDecrementado[i-3])){
            VectDecrementado[i] = (uint8_t)(VectDecrementado[i-1]+VectDecrementado[i-2]-VectDecrementado[i-3]);
        }else{
            VectDecrementado[i] = VectDecrementado[i] / 2;
        }
    }

    for(int i = 0 ; i < dim ; i++){
        if(VectDecrementado[i] == VectIncrementado[i]) iguales++;
    }

    printf("Elementos no procesados de VectIncrementado y VectDecrementado: 0, 1 y 2\n");
    printf("Cantidad de elementos iguales %d de %d\n", iguales, dim);
    printf("HPC: se pueden ejecutar en paralelo los recorridos de ambos vectores, "
       "porque son independientes tras la copia. Dentro de cada recorrido "
       "hay dependencias entre iteraciones, asi que no se deben repartir "
       "los indices directamente sin tenerlas en cuenta.\n");
    
    free(VectDecrementado);
    free(VectIncrementado);

    return 0;
}