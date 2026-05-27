#include <omp.h>
#include <stdio.h>
int fp=14;
int main()
{
	int iam=1, np=5,i;
	
//fp++;      	
#pragma omp threadprivate(fp)

	printf("\t\tP0: %d - %d  - %d\n",fp,iam,i);
fp++;

for (i=0;i<3;i++)
{

#pragma omp parallel num_threads(i+3) private(np,iam) shared(i) default(none) //copyin(fp)
{
    iam = omp_get_thread_num();  
    np = omp_get_num_threads();

	printf("\t\tP0: %d - %d  - %d\n",fp,iam,i);
	
    fp += (iam+1)*10;

    printf("\t\tP1: %d - %d  - %d\n",fp,iam,i);

}//parallel

fp+=1000;
printf("\t\tP2: %d - %d\n",fp,np);
}
}
