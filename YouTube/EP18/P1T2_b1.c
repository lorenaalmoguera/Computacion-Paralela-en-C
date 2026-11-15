#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

//#define TAM_V1 6000
#define MIN_V1 0
#define MAX_V1 10000
#define MIN_CEROS_V1 0

int sumaCifras(int numeroTesteo, int sumaCifras);
void rellenarVector(int *V, int tam);


int main(int argc, char *argv[])
{
    int *V1;
	double *V1D;
	int sizeV1;
    int i;
    int maxI = 0;
	int maxIG = 0;
	int numMy05 = 0;


	// Nuevas variables 
	int myrank;
	int np;
	int tamSlaves = 0;	
	MPI_Status status;
	
	
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

	int cntPerProcess[np];
	int dispPerProcess[np];

    // El proceso 0 intercepta argumento tamaño el argumento
    if (myrank == 0)
    {
        sizeV1 = atoi(argv[1]);
        printf("Proceso %d: sizeV1 = %d\n", myrank, sizeV1);
    }

	MPI_Bcast(&sizeV1,1,MPI_INT,0,MPI_COMM_WORLD);
	
	tamSlaves = sizeV1 / np;
	if (myrank == 0){
		for (i=0;i<np;i++)
		{
			cntPerProcess[i] = tamSlaves;
		}
		cntPerProcess[0] += sizeV1 % np;
		dispPerProcess[0] = 0;
		for (i=1;i<np;i++)
		{
			dispPerProcess[i] = dispPerProcess[i-1] + cntPerProcess[i-1];
		}	
	}
	
	MPI_Bcast(cntPerProcess,np,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(dispPerProcess,np,MPI_INT,0,MPI_COMM_WORLD);	
	
	tamSlaves = cntPerProcess[myrank];
	printf("Proceso %d: tamSlaves = %d\n", myrank, tamSlaves);

	// Reserva de memoria 
	if (myrank == 0)
	{
		V1 = (int *)malloc(sizeV1 * sizeof(int));
		V1D = (double *)malloc(sizeV1 * sizeof(double));
	}
	else
	{
		V1 = (int *)malloc(tamSlaves * sizeof(int));
		V1D = (double *)malloc(tamSlaves * sizeof(double));
	}
	
    srand(time(NULL));

    // Obtención de datos
	if (myrank == 0)
	{
		rellenarVector(V1,sizeV1);
	}
	
	//Calculos secuencial para comprobación
	maxI = 0; //Válido solo por restricción de valores
	if (myrank == 0)
	{
		for (i = 0; i < sizeV1; i++)
		{
			if (V1[i] > maxI) maxI = V1[i];
		}
		numMy05 = 0;
		for (i = 0; i < sizeV1; i++)
		{
			if ((V1[i] / (double)maxI) > 0.5) numMy05++;
		}
				
		printf("SEC: Máximo entero: %d\n", maxI);
		printf("SEC: Numero > 0.5: %d\n", numMy05);
		maxI = 0;
		numMy05 = 0;
	}	
	
	// Distribución de datos
	//CUIDADO CON V1
	MPI_Scatterv(V1,cntPerProcess,dispPerProcess, MPI_INT, V1, cntPerProcess[myrank], MPI_INT, 0, MPI_COMM_WORLD); 
	


	//Cálculo
	for (i = 0; i < tamSlaves; i++)
	{
		if (V1[i] > maxI) maxI = V1[i];
	}
	
	//Sincronización de resultados
	//CUIDADO con maxI
	//MPI_Reduce(&maxI,&maxIG,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
	MPI_Allreduce(&maxI,&maxIG,1,MPI_INT,MPI_MAX,MPI_COMM_WORLD);
	
	//Cálculo 
	numMy05 = 0;
	for (i = 0; i < tamSlaves; i++)
	{
		V1D[i] = V1[i] / (double)maxIG;
		if (V1D[i] > 0.5) numMy05++; 
	}
	
	printf("PARCIAL (de %d): Máximo entero: %d (global %d)\n", myrank, maxI, maxIG);
	printf("PARCIAL (de %d): Numero > 0.5: %d\n", myrank, numMy05);

	
	// Resultados
	MPI_Reduce(&numMy05,&numMy05,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Gatherv(V1D,cntPerProcess[myrank],MPI_INT, V1,cntPerProcess,dispPerProcess, MPI_INT, 0, MPI_COMM_WORLD); 
	
	
	if (myrank == 0)
	{
						
		printf("FINAL: Máximo entero: %d\n", maxIG);
		printf("FINAL: Numero > 0.5: %d\n", numMy05);
	}
	
    free(V1);
	free(V1D);
	
	MPI_Finalize();
    return 0;
}


int sumaCifras(int numeroTesteo, int sumaCifras)
{
	if (numeroTesteo == 0) return (-1); 
    int suma = 0;
    if (numeroTesteo < 0) numeroTesteo = -numeroTesteo;
    while (numeroTesteo > 0)
    {
        suma += numeroTesteo % 10;
        numeroTesteo /= 10;
    }
	return (suma==numeroTesteo);
}

void rellenarVector(int *V, int tam)
{
    int i;
    int posicion;
    int cerosColocados = 0;

    for (i = 0; i < tam; i++)
    {
        //V[i] = MIN_V1 + rand() % (MAX_V1 - MIN_V1 + 1);
		V[i] = rand() % MAX_V1;
    }

    /* Colocamos 100 ceros en posiciones aleatorias distintas */
    while (cerosColocados < MIN_CEROS_V1)
    {
        posicion = rand() % tam;

        if (V[posicion] != 0)
        {
            V[posicion] = 0;
            cerosColocados++;
        }
    }
}
