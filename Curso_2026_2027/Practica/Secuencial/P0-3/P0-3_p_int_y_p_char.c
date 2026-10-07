#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    const int tam = 100;
    int *v1;
    FILE *f;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <fichero>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* El fichero contiene 100 enteros; reservamos espacio para V1. */
    v1 = malloc((size_t)tam * sizeof *v1);
    if (v1 == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria para V1.\n");
        return EXIT_FAILURE;
    }

    f = fopen(argv[1], "rb");
    if (f == NULL) {
        perror("Error al abrir el fichero");
        free(v1);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < tam; i++) {
        fread(v1 + i, sizeof *v1, 1, f);
    }

    fclose(f);

    printf("Primeros cuatro enteros de V1:\n");
    for (int i = 0; i < 4; i++) {
        printf("V1[%d] = %d\n", i, v1[i]);
    }

    /* pcharV1 apunta al primer byte de V1. Solo se modifica ese byte. */
    char *pcharV1 = (char *)v1;
    *pcharV1 = 0;
    printf("\nTras escribir 0 mediante pcharV1 (un byte):\n");
    printf("*pcharV1 = %d\n", (int)*pcharV1);
    printf("V1[0] = %d\n", v1[0]);

    /* pintV1 apunta al primer entero completo de V1. */
    int *pintV1 = v1;
    *pintV1 = 0;
    printf("\nTras escribir 0 mediante pintV1 (un int):\n");
    printf("*pintV1 = %d\n", *pintV1);
    printf("V1[0] = %d\n", v1[0]);

    /*
     * Se hace esta escritura para observar el efecto pedido en la práctica.
     * Es una prueba experimental: acceder a los int mediante double* no es
     * portable según las reglas de C y el resultado depende de la plataforma
     * y del compilador.
     */
    double *pdoubleV1 = (double *)(void *)v1;
    *pdoubleV1 = 0.0;
    printf("\nTras escribir 0.0 mediante pdoubleV1 (un double):\n");
    printf("*pdoubleV1 = %f\n", *pdoubleV1);
    printf("V1[0] = %d\n", v1[0]);

    free(v1);
    return EXIT_SUCCESS;
}
