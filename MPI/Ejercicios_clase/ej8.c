#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
int main(int argc,char *argv[])
{
    int nproces, myrank, i, err, len;
    double data1,data2;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    err = MPI_Get_processor_name(name, &len);
    printf("Proceso %d  en procesador %s\n",myrank,name);
    data1 = 100 * (myrank +1);
    data2 = 0;
    err = MPI_Scan(&data1,&data2,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
    printf("Proceso %d. d1: %f - d2: %f.\n",myrank,data1,data2);  
    MPI_Finalize();
}

