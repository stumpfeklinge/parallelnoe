#include <stdio.h>
#include <mpi.h>
#include <math.h>

double f(double x)
{ 
	return 3*sqrt(x);
}

double g(double x)
{ 
	return cos(x);
}

int main(int argc, char *argv[])
{	int r, p,i,n=1000000000;
	double h,t,a=1.0,b=9.0;
	double c=0.0,d=1.0,h1;
	MPI_Status st;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&r); 	
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	printf("r=%d, p=%d\n",r,p);
	if (r==0)	
		t=MPI_Wtime();

	MPI_Barrier(MPI_COMM_WORLD);
	h=(b-a)/n;
	h1=(d-c)/n;
	double sumg=(g(a)+g(b))/2;
	double sumf=(f(a)+f(b))/2;
	for(i=r;i<=n;i+=p){
		sumf+=f(a+i*h);
		sumg+=g(c+i*h1);
	}
	sumf*=h;
	sumg*=h1;
	if (r!=0){
		MPI_Send(&sumf,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
		MPI_Send(&sumg,1,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
	}
	if (r==0) 
	{	
		double sf,sg;
		for(i=1;i<p;i++){
			MPI_Recv(&sf,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&st);
			MPI_Recv(&sg,1,MPI_DOUBLE,i,1,MPI_COMM_WORLD,&st);
			sumf+=sf;
			sumg+=sg;
		}
		t=MPI_Wtime()-t;
		printf("Значение разности интегралов = %lf\n", sumf-sumg);
		printf("Время выполнения = %lf\n", t);
	}
	MPI_Finalize();
}
