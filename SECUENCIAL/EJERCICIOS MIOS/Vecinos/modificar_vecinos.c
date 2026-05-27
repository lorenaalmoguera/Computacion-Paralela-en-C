#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>



int main(int argc, char* argv[]){

    int dim;
    char* filename;

    dim = atoi(argv[1]);
    filename = argv[2];

    double **m_og = (double**)malloc(dim*sizeof(double*));
    double **m = (double**)malloc(dim*sizeof(double*));
    double **m_sol = (double**)malloc(dim*sizeof(double*));
    
    for(int i = 0 ; i < dim ; i++) m[i] = (double*)malloc(dim*sizeof(double));
    for(int i = 0 ; i < dim ; i++) m_og[i] = (double*)malloc(dim*sizeof(double));
    for(int i = 0 ; i < dim ; i++) m_sol[i] = (double*)malloc(dim*sizeof(double));
    
    FILE * f = fopen(filename, "rb");

    for(int i = 0 ; i < dim ; i++) fread(m_og[i], sizeof(double), dim, f);
    
    fclose(f);
    
    for(int i = 0 ; i < dim ; i++){
        for(int j = 0 ; j < dim ; j++){
            *(*(m+i)+j) = *(*(m_og+i)+j);
        }
    }

    double media = 0;

    for(int i = 0 ; i < dim ; i++) for(int j = 0 ; j < dim ; j++) media += m[i][j];

    for(int i = 1 ; i < dim-1 ; i++){
        for(int j = 1 ; j < dim-1 ; j++){
            media = (m[i-1][j-1]+m[i-1][j]+m[i-1][j+1]+m[i][j-1]+m[i][j+1]+m[i+1][j-1]+m[i+1][j]+m[i+1][j+1]);
            media = media / 8;

            if(m[i][j] > media){
                m[i][j] = 0;
            }else if(m[i][j] < media){
                m[i][j] = -1;
            }
        }
    }
    
    // Multiplicación de matrices: m_sol = m_og * m
    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            m_sol[i][j] = 0;
            for(int k = 0; k < dim; k++) {
                m_sol[i][j] += m_og[i][k] * m[k][j];
            }
        }
    }
    
    // Guardar resultado
    FILE *f_out = fopen("resultado.bin", "wb");
    for(int i = 0; i < dim; i++) {
        fwrite(m_sol[i], sizeof(double), dim, f_out);
    }
    fclose(f_out);
    
    // Liberar memoria
    for(int i = 0; i < dim; i++) {
        free(m_og[i]);
        free(m[i]);
        free(m_sol[i]);
    }
    free(m_og);
    free(m);
    free(m_sol);
    
    return 0;
}