#include <omp.h>
#include <stdio.h>
int main()
{
   int iam =0, np = 1;
   int datos[100],i = 0, j = 0;	
#pragma omp parallel num_threads(4) shared(i) private(iam,np,j)
{
    iam = omp_get_thread_num();	
    np = omp_get_num_threads();
    #pragma omp single
    {
    printf ("El trabajo SINGLE solo lo realiza el hilo: %d\n",iam);
    for (j=0;j<10000;j++){
        i++;
	}
    i = 222;
    }
    printf ("Despues de  SINGLE el valor de i = %d para el hilo: %d\n",i,iam);
}//parallel

}
