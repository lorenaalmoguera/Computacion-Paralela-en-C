#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
int main(int argc,char *argv[])
{
    int nproces, myrank, i, err, len;
    double data1[6],data2[6];
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    err = MPI_Get_processor_name(name, &len);
    printf("Proceso %d  en procesador %s\n",myrank,name);
    for (i=0;i<3;i++){
        data1[i] = 100 * (myrank +1) + 10*i;
        data2[i] = 0;
    }
    err = MPI_Reduce(data1,data2,2,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    for (i=0;i<6;i++){
        printf("Proceso %d. d1: %f - d2: %f.\n",myrank,data1[i],data2[i]);  
     }
    MPI_Finalize();
}

