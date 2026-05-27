#include <omp.h>
#include <stdio.h>
int main()
{
   int iam =0, np = 1;
   int datos[100],i = 0, j = 0;	

#pragma omp parallel private(iam, np,i)
{
    iam = omp_get_thread_num();  
    #pragma omp for ordered
    for(i=0;i<5;i++)
    {
       #pragma omp ordered
       {
         printf("\tSoy el thread %d, antes del ordered en la iteracion %d\n",iam,i);
         printf("\t\tSoy el thread %d, actuando en la iteracion %d\n",iam,i);
         sleep(1);
       }
     }
}//parallel
}
