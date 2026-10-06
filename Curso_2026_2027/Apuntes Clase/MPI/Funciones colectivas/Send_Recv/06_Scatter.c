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

    int datos[8] = {10, 11, 20, 21, 30, 31, 40, 41};
    int local[2];
    const int cantidad = 2, etiqueta = 106;
    
    if (rank == 0) {
        for (int i = 0; i < cantidad; i++) local[i] = datos[i];
        for (int destino = 1; destino < size; destino++)
            MPI_Send(datos + destino * cantidad, cantidad, MPI_INT,
                     destino, etiqueta, MPI_COMM_WORLD);
    } else {
        MPI_Recv(local, cantidad, MPI_INT, 0, etiqueta, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }
    printf("P%d: %d %d\n", rank, local[0], local[1]);

    MPI_Finalize();
    return 0;
}
