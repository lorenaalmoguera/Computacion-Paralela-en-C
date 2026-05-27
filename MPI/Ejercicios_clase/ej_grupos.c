#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

 
int main(int argc,char *argv[])
{
    int nproces, myrank, i, err, len;
	int data1,data2;
	int data3=0,data4=0;
	int nproces2, myrank2;
	int nproces3, myrank3;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;
	int rangos1[4]={0,1,2};
    int rangos2[4]={7,5};
	MPI_Group G_original, G2, G3;
	MPI_Comm COMM2,COMM3;
	
    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	err = MPI_Get_processor_name(name, &len);
	//printf("Proceso %d  en procesador %s\n",myrank,name);
	//MPI_Barrier(MPI_COMM_WORLD); 
	
	
	MPI_Comm_group(MPI_COMM_WORLD, &G_original);
	
	MPI_Group_incl(G_original,3,rangos1,&G2);
	MPI_Group_incl(G_original,2,rangos2,&G3);

	MPI_Comm_create(MPI_COMM_WORLD,G2,&COMM2);
	MPI_Comm_create(MPI_COMM_WORLD,G3,&COMM3);
	
	MPI_Group_size(G2,&nproces2);
    MPI_Group_rank(G2,&myrank2);
	
    MPI_Group_size(G3,&nproces3);
    MPI_Group_rank(G3,&myrank3);
	
	data1=10;
	data2=100;
	if (myrank<3)
	MPI_Allreduce(&data1,&data3,1,MPI_INT,MPI_SUM,COMM2);
	//MPI_Allreduce(&data2,&data4,1,MPI_INT,MPI_SUM,COMM3);
	
    printf("Mis datos %d de %d. En el grupo G2 %d de %d. En grupo G3 %d de %d. Res %d %d \n",myrank,nproces,myrank2,nproces2,myrank3,nproces3,data3,data4);  
    MPI_Group_free(&G2);
	MPI_Group_free(&G3);
	MPI_Finalize();
}

