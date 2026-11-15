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
	int numMy05 = 0;


	// Nuevas variables 
	int myrank;
	int np;
	int tamSlaves = 0;	
	MPI_Status status;
	
	
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);


    // El proceso 0 intercepta argumento tamaño el argumento
    if (myrank == 0)
    {
        sizeV1 = atoi(argv[1]);
        printf("Proceso %d: sizeV1 = %d\n", myrank, sizeV1);
    }

	if (myrank == 0)
	{
	  for (i=1;i<np;i++)
	  {
		 MPI_Send(&sizeV1,1,MPI_INT,i,5,MPI_COMM_WORLD);
	  }
	}
	else
	{
	  MPI_Recv(&sizeV1,1,MPI_INT,0,5,MPI_COMM_WORLD,&status);
	}	

	//Reparto trabajo
	tamSlaves = sizeV1 / np;
	if (myrank == 0) tamSlaves += sizeV1 % np;

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
			if ((V1[i] / maxI) > 0.5) numMy05++;
		}
				
		printf("SEC: Máximo entero: %d\n", maxI);
		printf("SEC: Numero > 0.5: %d\n", numMy05);
		maxI = 0;
		numMy05 = 0;
	}	
	
	// Distribución de datos
	if (myrank == 0)
	{
	  int inicio = tamSlaves;
	  int sizetemp = sizeV1 / np;	
	  for (i=1;i<np;i++)
	  {
		 MPI_Send(&V1[inicio],sizetemp,MPI_INT,i,9,MPI_COMM_WORLD);
		 inicio += sizetemp;
	  }
	}
	else
	{
	  MPI_Recv(V1,tamSlaves,MPI_INT,0,9,MPI_COMM_WORLD,&status);
	}

	//Cálculo
	for (i = 0; i < tamSlaves; i++)
	{
		if (V1[i] > maxI) maxI = V1[i];
	}
	
	//Sincronización de resultados
	if (myrank > 0)
	{
		MPI_Send(&maxI,1,MPI_INT,0,10,MPI_COMM_WORLD);
	}
	else
	{
		int temp;
		for (i=1;i<np;i++)
	    {
			MPI_Recv(&temp,1,MPI_INT,i,10,MPI_COMM_WORLD,&status);
			if (temp > maxI) maxI = temp;
		}
	}	
	
	//Cálculo 
	numMy05 = 0;
	for (i = 0; i < tamSlaves; i++)
	{
		V1D[i] = V1[i] / (double)maxI;
		if (V1D[i] > 0.5) numMy05++;
	}
	
	printf("PARCIAL (de %d): Máximo entero: %d\n", myrank, maxI);
	printf("PARCIAL (de %d): Numero > 0.5: %d\n", myrank, numMy05);

	
	// Reducción de datos
	if (myrank > 0)
	{
		MPI_Send(&numMy05,1,MPI_INT,0,12,MPI_COMM_WORLD);
		MPI_Send(V1D,tamSlaves,MPI_DOUBLE,0,13,MPI_COMM_WORLD);
	}
	else
	{
		int temp;
		int inicio = tamSlaves;
		int sizetemp = sizeV1 / np;	
		for (i=1;i<np;i++)
	    {
			MPI_Recv(&temp,1,MPI_INT,i,12,MPI_COMM_WORLD,&status);
			numMy05 += temp;
			MPI_Recv(&V1D[inicio],sizetemp,MPI_DOUBLE,i,13,MPI_COMM_WORLD,&status);
			inicio += sizetemp;
		}
	}
	
	if (myrank == 0)
	{
						
		printf("FINAL: Máximo entero: %d\n", maxI);
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
