#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){

    char * filename;
    int tam;
    int *v1;
    int *v2;  
    int *datos;

    int max1 = -999999999;
    int max2 = -999999999;
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

    datos = (int*)malloc(2*tam*sizeof(int));
    v1 = datos;
    v2 = datos + tam;

    FILE *f = fopen(filename, "rb");

    for(int i = 0 ; i < tam ; i++){
        fread(v1+i, sizeof(int), 1, f);
    }

    for(int i = 0 ; i < tam ; i++){
        fread(v2+i, sizeof(int), 1, f);
    }

    fclose(f);

    for(int i = 0 ; i < tam ; i++){
        if(*(v1+i) > max1){
            max1 = *(v1+i);
        }
        if(*(v2+i)> max2){
            max2 = *(v2+i);
        }
    }

    if(max1>max2){
        maxglobal = max1;
    }else{
        maxglobal = max2;
    }

    printf("El numero maximo es %d\n", maxglobal);
    
    free(datos);
    return 0;
}
