#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
int main(int argc,char *argv[])
{
    int nproces, myrank, i, err, len;
    int send[20],receive[100];
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    err = MPI_Get_processor_name(name, &len);
    printf("Proceso %d  en procesador %s\n",myrank,name);
    for (i=0;i<20;i++){
         send[i]=0;receive[i]=0;
    }
    for (i=0;i<20;i++){
        send[i] = 10 * (myrank +1) + i;
        }
     err = MPI_Scatter(send,2,MPI_INTEGER,receive,2,MPI_INTEGER,0,MPI_COMM_WORLD);    
     for (i=0;i<8;i++){
        printf("Proceso %d. send: %d - receive: %d.\n",myrank,send[i],receive[i]);  
     }
    MPI_Finalize();
}

