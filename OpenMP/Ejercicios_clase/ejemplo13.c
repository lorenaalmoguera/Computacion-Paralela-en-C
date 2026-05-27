#include <omp.h>
#include <stdio.h>
int main()
{
   int iam =0, np = 1;
   int datos[100],i = 0, j = 0;	

#pragma omp parallel private(iam, np,i)
{
    iam = omp_get_thread_num();
    #pragma omp master 
    {
     printf("Thread %d ejecuta bloque 1.\n",iam); sleep(1);}
    #pragma omp master
    {
     printf("Thread %d ejecuta bloque 2.\n",iam); sleep(1);}
    #pragma omp master 
    {
     printf("Thread %d ejecuta bloque 3.\n",iam); sleep(1);}
    printf("Yo ya estoy fuera. Soy el thread %d.\n",iam);
}//parallel
}
