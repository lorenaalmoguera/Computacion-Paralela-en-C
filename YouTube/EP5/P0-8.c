#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char* argv[]){


    char *filename;
    int dim;

    filename = argv[1];
    dim = atoi(argv[2]);

    // RESERVA DE MEMORIA

    unsigned char ** MatTotal = (unsigned char**)malloc(dim*sizeof(unsigned char*));
    for(int i = 0 ; i < dim ; i++) MatTotal[i] = (unsigned char*)malloc(dim*sizeof(unsigned char));

    unsigned char **CuadSI = (unsigned char**)malloc((dim/2)*sizeof(unsigned char*));
    for(int i = 0 ; i < dim/2 ; i++) CuadSI[i] = (unsigned char*)malloc((dim/2)*sizeof(unsigned char));

    unsigned char **CuadSD = (unsigned char**)malloc((dim/2)*sizeof(unsigned char*));
    for(int i = 0 ; i < dim/2 ; i++) CuadSD[i] = (unsigned char*)malloc((dim/2)*sizeof(unsigned char));

    unsigned char **CuadII = (unsigned char**)malloc((dim/2)*sizeof(unsigned char*));
    for(int i = 0 ; i < dim/2 ; i++) CuadII[i] = (unsigned char*)malloc((dim/2)*sizeof(unsigned char));
    
    unsigned char **CuadID = (unsigned char**)malloc((dim/2)*sizeof(unsigned char*));
    for(int i = 0 ; i < dim/2 ; i++) CuadID[i] = (unsigned char*)malloc((dim/2)*sizeof(unsigned char));


    FILE * f = fopen(filename, "rb");
    for(int i = 0 ; i < dim ; i++) fread(MatTotal[i], sizeof(unsigned char), dim, f);
    fclose(f);

    // SI SI SD SD      
    // SI SI SD SD      
    // II II ID ID      
    // II II ID ID      

    // PASAR TODO A SUBMATRICES
    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            if(i < dim/2 && j < dim/2) {
                // Superior Izquierdo: copiar directamente
                CuadSI[i][j] = MatTotal[i][j]; // Copias directamente: CuadSI[i][j]
            } else if(i < dim/2 && j >= dim/2) {
                // Superior Derecho: restar dim/2 de la columna
                CuadSD[i][j - dim/2] = MatTotal[i][j]; // Restas dim/2 de la columna: CuadSD[i][j - dim/2] porque en CuadSD solo tienes índices 0-255, no 256-511
            } else if(i >= dim/2 && j < dim/2) {
                // Inferior Izquierdo: restar dim/2 de la fila
                CuadII[i - dim/2][j] = MatTotal[i][j]; // Restas dim/2 de la fila: CuadII[i - dim/2][j]
            } else {
                // Inferior Derecho: restar dim/2 de fila y columna
                CuadID[i - dim/2][j - dim/2] = MatTotal[i][j]; // Restas dim/2 de ambas: CuadID[i - dim/2][j - dim/2]
            }
        }
    }


}