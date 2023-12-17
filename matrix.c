#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{	
	int r, p,i,j,n=2000;
	double sum=0,*a, *b, *c,t, t1,t2;
	double s,ss;
	a=(double *)calloc(n*n,sizeof(double));
	b=(double *)calloc(n*n,sizeof(double));
	c=(double *)calloc(n*n,sizeof(double));
	
	MPI_Status st;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&r);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	if (r==0)
		{
			for(i=0;i<n;i++)
				for(j=0;j<n;j++)
					a[i*n+j]=rand() %100 +-100;
			for(i=0;i<n;i++)
				for(j=0;j<n;j++)
					b[i*n+j]=rand() %100 +-100;
		}

	MPI_Bcast(&n,1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(a,n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(b,n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(c,n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	t1=MPI_Wtime();
	MPI_Barrier(MPI_COMM_WORLD);
	int kvo=n/p; int in=r*kvo; int ik;
	if (r==p-1) 
		ik=n;
	else 
		ik=in+kvo;
	int pp;
	for(i=in;i<ik;i++)
		for(j=0;j<n;c[i*n+j]=s,j++)
			for(s=0,pp=0;pp<n;pp++)
				s+=a[i*n+pp]*a[pp*n+j]+b[i*n+pp]*b[pp*n+j];
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	if (r==0) 
	{
		int ii;
		printf("Сборка матрицы, вошли в цикл\n");
		for( ii=1;ii<p;ii++)
		{
			MPI_Recv(&in,1,MPI_INT,ii,1,MPI_COMM_WORLD,&st);
			MPI_Recv(&ik,1,MPI_INT,ii,2,MPI_COMM_WORLD,&st);
			MPI_Recv(c+ik*n,(ik-in)*n,MPI_DOUBLE,ii,0,MPI_COMM_WORLD,&st);
		}
		printf("Сборка матрицы, вышли из цикла\n");
		t2=MPI_Wtime();
		t=t2-t1;
		
		printf("Итоговое время счёта=%lf c.\n", t);	
	}
	else
	{
		MPI_Send(&in,1,MPI_INT,0,1,MPI_COMM_WORLD);
		MPI_Send(&ik,1,MPI_INT,0,2,MPI_COMM_WORLD);
		MPI_Send(c+in*n,(ik-in)*n,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
	}

	
	MPI_Finalize();
}
