#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 4) {
        if (rank == 0) fprintf(stderr, "Ejecuta con 4 procesos.\n");
        MPI_Finalize(); return 1;
    }

    int sendbuf[8], recvbuf[8];
    for (int dst = 0; dst < size; ++dst) {
        sendbuf[2 * dst] = 100 * (rank + 1) + 10 * dst;
        sendbuf[2 * dst + 1] = sendbuf[2 * dst] + 1;
    }
    const int tag = 133;
    recvbuf[2 * rank] = sendbuf[2 * rank];
    recvbuf[2 * rank + 1] = sendbuf[2 * rank + 1];

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (rank == i) {
                MPI_Send(&sendbuf[2 * j], 2, MPI_INT, j, tag, MPI_COMM_WORLD);
                MPI_Recv(&recvbuf[2 * j], 2, MPI_INT, j, tag,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else if (rank == j) {
                MPI_Recv(&recvbuf[2 * i], 2, MPI_INT, i, tag,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&sendbuf[2 * i], 2, MPI_INT, i, tag, MPI_COMM_WORLD);
            }
        }
    }

    printf("P%d:", rank);
    for (int i = 0; i < 2 * size; ++i) printf(" %d ", recvbuf[i]);
    printf("\n");
    MPI_Finalize();
    return 0;
}
