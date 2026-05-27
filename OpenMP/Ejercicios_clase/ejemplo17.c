#include <omp.h>
#include <stdio.h>
#define tam 100
int main()
{
   int iam =0, np = 1;
   int datos[100],i = 0, j = 0; 
   int suma[4];
   int tam_bloque,ini,fin,suma_global=0;
    for(i=0;i<tam;i++) datos[i] = i;
#pragma omp parallel num_threads(4) private(iam,np,i,ini,fin) shared(j,suma,datos,suma_global)
{
    iam = omp_get_thread_num();  
    np = omp_get_num_threads();
    tam_bloque = tam/np;  ini = tam_bloque * iam;  fin = tam_bloque * (iam + 1); suma[iam] = 0;
    sleep(iam*2 + 1);
    for(i=ini;i<fin;i++)   {     suma[iam] += datos[i]; }
    #pragma omp barrier
    #pragma omp master
    {
        for (i=0;i<np;i++) suma_global+= suma[i];}
    printf("Valor final de suma global %d (hilo: %d).\n",suma_global,iam);
}//parallel
}



