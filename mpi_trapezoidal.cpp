//#include <stdio.h>
#include <iostream>
#include <string.h>
#include <mpi.h>

/* For strlen
*/
/* For MPI functions, etc */
using namespace std;

double f(double x){
	return x*x + 1.0;
}
double Trap(
			double local_a, /* in */
			double local_b, /* in */
			int local_n, /* in */
			double h /* in */){
	
	double my_area, x;
	int i;
	my_area = (f(local_a) + f(local_b))/2.0;

	for (i = 1; i <= local_n - 1; i++) {
		x = local_a + i*h;
		my_area += f(x);
	}
	my_area = my_area*h;
	return my_area;
}


int main(int argc, char const *argv[]){
	int my_rank, comm_sz, n = 1024, local_n;
	double a = 0.0, b = 3.0, h, local_a, local_b;
	double local_int, total_int;
	int source;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	
	h = (b - a)/n; /* h is the same for all processes */
	local_n = n/comm_sz; /* So is the number of trapezoids */
	
	local_a = a + my_rank*local_n*h;
	local_b = local_a + local_n*h;
	local_int = Trap(local_a, local_b, local_n, h);

	if (my_rank != 0) {
		MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	} else {
		total_int = local_int;
		for (source = 1; source < comm_sz; source++) {
			MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total_int += local_int;
		}
	}
	if (my_rank == 0) {
		printf("With n = %d trapezoids, our my_area\n", n);
		printf("of the integral from %f to %f = %.15f\n",a, b, total_int);
	}
	MPI_Finalize();//ya no utilizará MPI
	return 0;
} /* main */

/*
mpiCC -g -Wall mpi_trapezoidal.cpp  -o m
mpirun -np 2 ./m
*/