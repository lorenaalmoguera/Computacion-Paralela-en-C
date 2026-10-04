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

    /* El fichero contiene 100 enteros; se reserva el espacio de V1. */
    v1 = malloc((size_t)tam * sizeof *v1);

    f = fopen(argv[1], "rb");
    if (f == NULL) {
        perror("No se pudo abrir el fichero");
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

    /* pcharV1 apunta al mismo comienzo que V1, visto byte a byte. */
    char *pcharV1 = (char *)v1;
    *pcharV1 = 0;
    printf("Tras poner a cero el primer byte mediante pcharV1:\n");
    printf("*pcharV1 = %d\n", (int)*pcharV1);
    printf("V1[0] = %d\n", v1[0]);

    /* pintV1 apunta al mismo primer elemento, como un int. */
    int *pintV1 = v1;
    *pintV1 = 0;
    printf("Tras poner a cero el primer int mediante pintV1:\n");
    printf("*pintV1 = %d\n", *pintV1);
    printf("V1[0] = %d\n", v1[0]);

    /* El puntero puede señalar el comienzo, pero no se debe desreferenciar:
     * V1 contiene objetos int y acceder a ellos como double no es válido
     * según C estándar (además, un double puede abarcar varios int).
     */
    double *pdoubleV1 = (double *)(void *)v1;
    printf("pdoubleV1 apunta al comienzo de V1: %p\n", (void *)pdoubleV1);

    free(v1);
    return EXIT_SUCCESS;
}
