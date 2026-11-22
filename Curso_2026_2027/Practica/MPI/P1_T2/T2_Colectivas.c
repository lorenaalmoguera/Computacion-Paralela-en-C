#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char* argv[]){


    int myrank, nproces, dim;
    int *vector = NULL;
    double *resultado = NULL;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproces);

    if(myrank == 0){
        dim = atoi(argv[1]);

        srand(time(NULL));
        vector = (int*)malloc(dim*sizeof(int));
        resultado = (double*)malloc(dim*sizeof(double));

        for(int i = 0 ; i < dim ; i++){
            *(vector+i) = rand() % 100001;
        }
    }

    MPI_Bcast(&dim, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int *send_counts = (int*)malloc(nproces*sizeof(int));
    int *displ_counts = (int*)malloc(nproces*sizeof(int));

    int num_elementos = dim / nproces;

    if(myrank == 0){
        for(int i = 0 ; i < nproces; i++){
            send_counts[i] = num_elementos;
        }
        send_counts[0] += dim % nproces;
        displ_counts[0] = 0;
        for(int i =  1 ; i < nproces ; i++){
            displ_counts[i] = displ_counts[i-1] + send_counts[i-1];
        }
    }

    MPI_Bcast(send_counts, nproces, MPI_INT, 0, MPI_COMM_WORLD);

    num_elementos = send_counts[myrank];

    int * vector_parcial = (int*)malloc(num_elementos*sizeof(int));
    double * resultado_parcial = (double*)malloc(num_elementos*sizeof(double));

    MPI_Scatterv(vector, send_counts, displ_counts, MPI_INT, vector_parcial, send_counts[myrank], MPI_INT, 0, MPI_COMM_WORLD);

    int mi_maximo = 0;
    for(int i = 0 ; i < num_elementos ; i++){
        if(vector_parcial[i] > mi_maximo) mi_maximo = vector_parcial[i];
    }

    int maximo;
    MPI_Reduce(&mi_maximo, &maximo, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Bcast(&maximo, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int mi_cont = 0;

    for(int i = 0 ; i < num_elementos ; i++){
        if(maximo == 0){
            resultado_parcial[i] = 0.0;
        }else{
            resultado_parcial[i] = (double)vector_parcial[i] / maximo;
        }
        if(resultado_parcial[i] > 0.5) mi_cont++;
    }

    int contador;
    MPI_Reduce(&mi_cont, &contador, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Gatherv(resultado_parcial, num_elementos, MPI_DOUBLE, resultado, send_counts, displ_counts, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(myrank == 0){
        for(int i = 0 ; i < dim ; i++){
            printf("%.2f ", resultado[i]);
            
        }
        printf("\nTotal de valores mayores a 0.5: %d\n", contador);
        printf("Maximo del vector: %d\n", maximo);

        char * nombre = "resultado_colectivas.bin";
        if(argc == 3){
            nombre = argv[2];
        }
        FILE *fichero = fopen(nombre, "wb");
        fwrite(resultado, sizeof(double), dim, fichero);
        fclose(fichero);
    }

    free(resultado);
    free(vector);
    free(vector_parcial);
    free(resultado_parcial);
    free(displ_counts);
    free(send_counts);

    MPI_Finalize();

    return 0;
}