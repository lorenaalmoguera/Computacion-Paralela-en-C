#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 4) {
        if (rank == 0) fprintf(stderr, "Ejecuta con 4 procesos.\n");
        MPI_Finalize();
        return 1;
    }

    int entrada[4] = {1, 2, 3, 4}, local;
    int counts[4] = {1, 1, 1, 1};
    const int llegada = 108, reparto = 109;
    /* Todos aportan sum(counts) = 4 elementos. */
    const int longitud = 4;
    
    if (rank == 0) {
        int suma[4], recibido[4];
        for (int i = 0; i < longitud; i++) suma[i] = entrada[i];
        /* Fase 1: sumar los vectores elemento a elemento. */
        for (int origen = 1; origen < size; origen++) {
            MPI_Recv(recibido, longitud, MPI_INT, origen, llegada,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < longitud; i++) suma[i] += recibido[i];
        }
        /* Fase 2: repartir bloques consecutivos del vector suma. */
        local = suma[0];
        int desplazamiento = counts[0];
        for (int destino = 1; destino < size; destino++) {
            MPI_Send(suma + desplazamiento, counts[destino], MPI_INT,
                     destino, reparto, MPI_COMM_WORLD);
            desplazamiento += counts[destino];
        }
    } else {
        MPI_Send(entrada, longitud, MPI_INT, 0, llegada, MPI_COMM_WORLD);
        MPI_Recv(&local, counts[rank], MPI_INT, 0, reparto,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    printf("P%d: %d\n", rank, local);

    MPI_Finalize();
    return 0;
}
