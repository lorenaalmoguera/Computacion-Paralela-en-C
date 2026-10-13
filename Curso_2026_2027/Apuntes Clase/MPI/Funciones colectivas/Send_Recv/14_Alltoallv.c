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

    int sendcounts[4], sdispls[4], recvcounts[4], rdispls[4];
    int sendbuf[16], recvbuf[16];
    int send_total = 0, recv_total = 0;
    for (int j = 0; j < size; ++j) {
        sendcounts[j] = (rank + 2 * j) % 4;
        sdispls[j] = send_total;
        send_total += sendcounts[j];
        recvcounts[j] = (j + 2 * rank) % 4;
        rdispls[j] = recv_total;
        recv_total += recvcounts[j];
    }
    for (int dst = 0; dst < size; ++dst)
        for (int k = 0; k < sendcounts[dst]; ++k)
            sendbuf[sdispls[dst] + k] = 100 * rank + 10 * dst + k;

    const int tag = 144;
    for (int k = 0; k < sendcounts[rank]; ++k)
        recvbuf[rdispls[rank] + k] = sendbuf[sdispls[rank] + k];

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (rank == i) {
                MPI_Send(&sendbuf[sdispls[j]], sendcounts[j], MPI_INT,
                         j, tag, MPI_COMM_WORLD);
                MPI_Recv(&recvbuf[rdispls[j]], recvcounts[j], MPI_INT,
                         j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else if (rank == j) {
                MPI_Recv(&recvbuf[rdispls[i]], recvcounts[i], MPI_INT,
                         i, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&sendbuf[sdispls[i]], sendcounts[i], MPI_INT,
                         i, tag, MPI_COMM_WORLD);
            }
        }
    }

    printf("P%d:", rank);
    for (int i = 0; i < recv_total; ++i) printf(" %d ", recvbuf[i]);
    printf("\n");
    MPI_Finalize();
    return 0;
}
