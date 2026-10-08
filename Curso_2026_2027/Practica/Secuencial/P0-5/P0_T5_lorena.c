#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){


    char* filename;
    int tam = 1000;
    int div;

    int num_elem, resto;

    if(argc < 3){
        printf("Uso <fichero> <div>\n");
        return -1;
    }

    filename = argv[1];
    div = atoi(argv[2]);

    if(div > 10 || div < 2){
        printf("Dato no permitido\n");
        return -1;
    }


    int *v1 = (int*)malloc(tam*sizeof(int));
    int *maximos = (int*)malloc(div*sizeof(int));
    int **partes = (int**)malloc((div-1)*sizeof(int*));

    num_elem = tam / div;
    resto = tam % div;

    FILE * f = fopen(filename, "rb");
    for(int i = 0 ; i < tam ; i++){
        fread(v1+i, sizeof(int), 1, f);
    }
    fclose(f);

    int inicio = 0;

    for(int i = 0 ; i < div ; i++){
        int tam_parte = num_elem;
        if(i == div-1) tam_parte = tam_parte + resto;

        if(i > 0){
            partes[i-1] = malloc(tam_parte*sizeof(int));

            for(int j = 0 ; j < tam_parte ; j++){
                partes[i-1][j] = v1[inicio+j]; // *(*(partes+i-1)+j) = *(v1+inicio+j);
            }
        }
        inicio = inicio + tam_parte;
    }


    inicio = 0;

    for(int i = 0 ; i < div ; i++){
        int tam_parte = num_elem;

        if(i == div-1) tam_parte = tam_parte + resto;

        int max_parte;

        if(i == 0){
            max_parte = v1[inicio];
        }else{
            max_parte = partes[i-1][0];
        }

        for(int j = 1 ; j < tam_parte ; j++){
            int valor;

            if(i == 0){
                valor = v1[inicio+j];
            }else{
                valor = partes[i-1][j];
            }

            if(valor > max_parte){
                max_parte = valor;
            }
        }

        maximos[i] = max_parte;

        printf("Parte %d : indice de contenido = %d , cantidad de elementos = %d , maximo = %d\n", i+1, inicio, tam_parte, maximos[i]);

        inicio = inicio + tam_parte;
    }

    int max_global = maximos[0];

    for(int i = 1 ; i < div ; i++){
        if(maximos[i] > max_global) max_global = maximos[i];
    }

    printf("El maximo global es: %d\n", max_global);

    for(int i = 0 ; i < div - 1 ; i++){
        free(partes[i]);
    }

    free(maximos);
    free(partes);
    free(v1);

    return 0;
}