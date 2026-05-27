#include <omp.h>
#include <stdio.h>
int main()
{
   int iam =0, np = 1;
   int datos[100],i = 0, j = 0;	
   for (i=0;i<5;i++) datos[i]=0;
#pragma omp parallel num_threads(4) private(iam, np,i) shared(j,datos)
{
    iam = omp_get_thread_num();  
    datos[iam] = 10*iam +1;
    #pragma omp flush(datos)
    printf("\tSoy el thread %d. Array: %d %d %d %d\n",iam,datos[0],datos[1],datos[2],datos[3]);
}//parallel
printf("\t\tSoy el thread %d, valor FINAL de j: %d\n",iam,j);
}
