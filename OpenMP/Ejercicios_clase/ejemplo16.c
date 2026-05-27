#include <omp.h>
#include <stdio.h>
int main()
{
   int iam =0, np = 1;
   int datos[100],i = 0, j = 0;	

#pragma omp parallel num_threads(4) private(iam, np,i) shared(j)
{
    iam = omp_get_thread_num();  
    for(i=0;i<5;i++)
    {
       #pragma omp critical
       {
         j++;
       }
     }
     printf("\tSoy el thread %d, valor actual de j: %d\n",iam,j);
}//parallel
printf("\t\tSoy el thread %d, valor FINAL de j: %d\n",iam,j);
}
