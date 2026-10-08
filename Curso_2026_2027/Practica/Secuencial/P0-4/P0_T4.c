#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    char *filename;
    int tam = 1000;
    int div;
    int *v1;
    int *v_res;

    if (argc < 3) {
        printf("Uso: %s <fichero> <div>\n", argv[0]);
        return -1;
    }

    filename = argv[1];
    div = atoi(argv[2]);

    if (div > 10 || div < 1) {
        printf("No se permite la division del vector en esa cantidad de partes.\n");
        return -1;
    }

    /* Reserva de memoria */
    v1 = (int *)malloc(tam * sizeof(int));
    v_res = (int *)malloc(div * sizeof(int));

    FILE *f = fopen(filename, "rb");

    for (int i = 0; i < tam; i++) {
        fread(v1 + i, sizeof(int), 1, f);
    }

    fclose(f);

    int num_d = tam / div;
    int resto = tam % div;
    int inicio = 0;
    int max_global = *(v1 + 0);

    printf("Maximo en cada una de las %d partes:\n", div);

    for (int parte = 0; parte < div; parte++) {
        int tam_parte = num_d + (parte < resto);
        int max_parte = *(v1 + inicio);

        for (int i = inicio + 1; i < inicio + tam_parte; i++) {
            if (*(v1 + i) > max_parte) {
                max_parte = *(v1 + i);
            }
        }

        *(v_res + parte) = max_parte;

        if (*(v_res + parte) > max_global) {
            max_global = *(v_res + parte);
        }

        printf("Parte %d: indice de comienzo = %d, elementos = %d, maximo = %d\n",
               parte + 1, inicio, tam_parte, *(v_res + parte));

        inicio = inicio + tam_parte;
    }

    printf("Maximo global = %d\n", max_global);

    free(v1);
    free(v_res);

    return 0;
}
