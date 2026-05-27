#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define tam 10
#define num_max_threads 12

int main()
{
   int iam,np;
   int *vector1,*vector2,*vector3,*vector4;
   double *vector_d5;
   int prod_int[num_max_threads];
   int i,ini,fin,tamBloque;
   int pInterno1V,pInterno2V;
   int max_value[num_max_threads];
   int maximo;

   vector1 = (int*)malloc(tam*sizeof(vector1[0]));
   vector2 = (int*)malloc(tam*sizeof(vector2[0]));
   vector3 = (int*)malloc(tam*sizeof(vector3[0]));
   vector4 = (int*)malloc(tam*sizeof(vector4[0]));

   vector_d5 = (double*)malloc(tam*sizeof(vector_d5[0]));

   for (i=0;i<tam;i++){
       vector1[i]=i;vector2[i]=tam-i;}
//       vector1[i]=1;vector2[i]=1;}
   memset(prod_int,0,num_max_threads*sizeof(prod_int[0])); 


#pragma omp parallel num_threads(4) private(iam,np,i,ini,fin) default(shared)
{
    iam = omp_get_thread_num();  
    np = omp_get_num_threads();

    tamBloque = tam/np;  ini = tamBloque * iam;  fin = tamBloque * (iam + 1);
    printf("Ini, Fin y tambloque: %d - %d % d del hilo %d\n\n",ini,fin,tamBloque,iam);
    prod_int[iam] = 0;
    for(i=ini;i<fin;i++)   { prod_int[iam] += vector1[i]*vector1[i]; }
    #pragma omp barrier
    #pragma omp single
    {
        for (i=0;i<np;i++) pInterno1V+= prod_int[i];}
}//parallel

#pragma omp parallel for  num_threads(3) default(shared) private(i) reduction(+:pInterno2V)
    for(i=0;i<tam;i++)   { pInterno2V += vector1[i]*vector2[i]; }


#pragma omp parallel  num_threads(4) default(shared) private(i) 
{
    iam = omp_get_thread_num();  
    np = omp_get_num_threads();
    #pragma omp for private(i)
         for(i=0;i<tam;i++)   { vector3[i] = vector1[i] + vector2[i]; }
}

#pragma omp parallel  num_threads(3) default(shared) private(i,iam,np) 
{
    iam = omp_get_thread_num();  
    np = omp_get_num_threads();
    if (iam == 0){
         for(i=0;i<tam/3;i++)   { vector4[i] = vector1[i] - vector2[i]; } }
    else if (iam == 1){
         for(i=tam/3;i<2*tam/3;i++)   { vector4[i] = vector1[i] - vector2[i]; } }
    else {
         for(i=2*tam/3;i<tam;i++)   { vector4[i] = vector1[i] - vector2[i]; } }
   

}


#pragma omp parallel  num_threads(4) default(shared) private(i,iam,np,maximo) 
{
    iam = omp_get_thread_num();  
    np = omp_get_num_threads();
	max_value[iam] = 0;
	#pragma omp for private(i)
		for(i=0;i<tam;i++)   {
			if (vector2[i] > max_value[iam]) max_value[iam] = vector2[i];
		}
	
	#pragma omp single copyprivate(maximo) 
	{
		maximo = max_value[0];
		for (i=1;i<np;i++){ if (max_value[i] > maximo) maximo = max_value[i];}
	}

	#pragma omp for
		for(i=0;i<tam;i++)   {
			vector_d5[i] = (double) vector2[i] / maximo ;
		}
}



    printf ("Producto Interno 1V: %d \n",pInterno1V);
    printf ("Producto Interno 2V: %d \n",pInterno2V);
    printf ("Vector 1:  \n");
    for(i=0;i<tam;i++) {printf(" - %d - ",vector1[i]); }
    printf ("\nVector 2:  \n");
    for(i=0;i<tam;i++) {printf(" - %d - ",vector2[i]); }
    printf ("\nVector Suma:  \n");
    for(i=0;i<tam;i++) {printf(" - %d - ",vector3[i]); }
    printf ("\nVector Resta:  \n");
    for(i=0;i<tam;i++) {printf(" - %d - ",vector4[i]); }
    printf ("\nVector Escalado:  \n");
    for(i=0;i<tam;i++) {printf(" - %f - ",vector_d5[i]); }
    printf("\n\n");
    free(vector1);
    free(vector2);
    free(vector3);
    free(vector4);

}



