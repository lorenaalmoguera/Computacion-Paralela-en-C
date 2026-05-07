    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>

    int main(int argc, char * argv[]) {

        char* fichero;
        int filas, columnas;

        if(argc < 4) return 1;

        filas = atoi(argv[1]);
        columnas = atoi(argv[2]);
        fichero = argv[3];

        float **matriz = (float**)malloc(filas * sizeof(float*));
        
        for(int i = 0 ; i < filas ; i++) {
            matriz[i] = (float*)malloc(columnas * sizeof(float));
        }

        srand(time(NULL));

        // Rellenar matriz
        for(int i = 0 ; i < filas ; i++) {
            for(int j = 0 ; j < columnas ; j++) {
                matriz[i][j] = (float)rand() / RAND_MAX * 100;
            }
        }

        // Abrir fichero
        FILE *f = fopen(fichero, "wb");

        if(f == NULL) {
            printf("Error al abrir el fichero\n");
            return 1;
        }

        // Escribir matriz en fichero
        for(int i = 0 ; i < filas ; i++) {

            fwrite(matriz[i], sizeof(float), columnas, f);

        }

        // Cerrar fichero
        fclose(f);

        // Liberar memoria
        for(int i = 0 ; i < filas ; i++) {
            free(matriz[i]);
        }

        free(matriz);

        return 0;
    }