#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){


    char* filename;
    int dim = 512;
    int partes;
    int **m;


    if(argc < 3){
        printf("uso: <filename> <partes>\n");
        return -1;
    }

    filename = argv[1];
    partes = atoi(argv[2]);

    if(partes < 2 || partes > 5){
        printf("dato no permitido\n");
        return -1;
    }
    
    m = (int**)malloc(dim*sizeof(int*));
    for(int i = 0 ; i < dim ; i++){
        m[i] = (int*)malloc(dim*sizeof(int));
    }

    



    return 0;
}