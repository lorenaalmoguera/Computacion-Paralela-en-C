#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

double desviacion_tipica(unsigned char *izq, unsigned char *der, int dim_cuad){
    double suma = 0, sum_cuadrados = 0;
    int dim_completo = dim_cuad * 2;
    
    // Calcular media (sumando ambos cuadrantes)
    for(int i = 0; i < dim_cuad; i++) {
        suma += izq[i];
        suma += der[i];
    }
    double media = suma / dim_completo;
    
    // Calcular suma de (x - media)² para toda la fila
    for(int i = 0; i < dim_cuad; i++) {
        double diferencia = izq[i] - media;
        sum_cuadrados += diferencia * diferencia;
        diferencia = der[i] - media;
        sum_cuadrados += diferencia * diferencia;
    }
    
    return sqrt(sum_cuadrados / dim_completo);
}

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

    double desv;

    // Calcular desviaciones usando los cuadrantes
    // Fila 0: parte superior izquierda [0] y superior derecha [0]
    desv = desviacion_tipica(CuadSI[0], CuadSD[0], dim/2);
    printf("Desviacion tipica de la fila 0: %.10f\n", desv);
    
    // Fila 1: parte superior izquierda [1] y superior derecha [1]
    desv = desviacion_tipica(CuadSI[1], CuadSD[1], dim/2);
    printf("Desviacion tipica de la fila 1: %.10f\n", desv);
    
    // Fila 255: parte superior izquierda [255] y superior derecha [255]
    desv = desviacion_tipica(CuadSI[dim/2 - 1], CuadSD[dim/2 - 1], dim/2);
    printf("Desviacion tipica de la fila 255: %.10f\n", desv);
    
    // Fila 256: parte inferior izquierda [0] e inferior derecha [0]
    desv = desviacion_tipica(CuadII[0], CuadID[0], dim/2);
    printf("Desviacion tipica de la fila 256: %.10f\n", desv);
    
    // Fila 511: parte inferior izquierda [255] e inferior derecha [255]
    desv = desviacion_tipica(CuadII[dim/2 - 1], CuadID[dim/2 - 1], dim/2);
    printf("Desviacion tipica de la fila 511: %.10f\n", desv);
    
    // Fila 512 (si existe, sería dim): parte inferior izquierda [256] e inferior derecha [256]
    // Nota: Esto solo funciona si dim >= 1024. Para dim = 512, esta fila no existe.
    if(dim >= 1024) {
        desv = desviacion_tipica(CuadII[dim/2 - 1], CuadID[dim/2 - 1], dim/2);
        printf("Desviacion tipica de la fila 512: %.10f\n", desv);
    }

    for(int i = 0 ; i < dim ; i++){
        free(MatTotal[i]);
    }
    free(MatTotal);

    for(int i = 0; i < dim/2 ; i++){
        free(CuadID[i]);
        free(CuadII[i]);
        free(CuadSD[i]);
        free(CuadSI[i]);
    }
    free(CuadID);
    free(CuadII);
    free(CuadSD);
    free(CuadSI);

    return 0;

}