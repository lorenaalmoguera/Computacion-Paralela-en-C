#include <omp.h>
#include <stdio.h>
int main()
{
   int iam =0, np = 1;
   int datos[100],i;

#pragma omp parallel num_threads(2) private(iam)
{
    iam = omp_get_thread_num();	
    #pragma omp sections
    {
        #pragma omp section
        {
        printf("El thread %d realiza la seccion 1. \n",iam);
        for (i=0;i<100000;i++) datos[0]++;
        }
        #pragma omp section
        printf("El thread %d realiza la seccion 2. \n",iam);
        #pragma omp section
        printf("El thread %d realiza la seccion 3. \n",iam);
        #pragma omp section
        printf("El thread %d realiza la seccion 4. \n",iam);
        #pragma omp section
        printf("El thread %d realiza la seccion 5. \n",iam);
    }//sections
}//parallel

}
