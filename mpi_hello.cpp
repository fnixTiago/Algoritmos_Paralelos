//#include <stdio.h>
#include <iostream>
#include <string.h>
#include <mpi.h>

/* For strlen
*/
/* For MPI functions, etc */
using namespace std;

const int MAX_STRING = 100;
int main(void) {
	char greeting[MAX_STRING];
	//string greeting="";
	int comm_sz; /* Number of processes */
	int my_rank; /* My process rank */
	MPI_Init(NULL, NULL);//indica que usará MPI
	//MPI_COMM_WORLD: Comunicador de todos los procesos
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);//devuelve en su segundo argumento el número de procesos en el comunicador
	/*
		int MPI_Comm_size(MPI_Comm comm ,  //ingreso 
						  int* comm sz_p ); //salida
	*/
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);//devuelve en su segundo argumento el proceso de llamada 'rango en el comunicador.
	/*
		int MPI_Comm_rank(MPI_Comm comm,   //ingreso 
						  int* my_rank_p ); //salida
	*/
	//printf("\nsize: %d, rank: %d \n",nro_size,nro_ranks);
	printf("Mi rank es: %d\n",my_rank );
	if (my_rank != 0) {
		//greeting = "Hola from process " + my_rank<<" of "<<comm_sz<<"!";
		sprintf(greeting, "Hola from IF process %d of %d!",my_rank, comm_sz);//similar al printf, solo que este lo guarda en una cadena
		MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	} else {
		printf("Hola from else process %d of %d!\n", my_rank, comm_sz);
		for (int q = 1; q < comm_sz; q++) {
			MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("****: %s\n", greeting);
		}
 	}
	MPI_Finalize();//ya no utilizará MPI
	//cout<<"\nGOOD\n";
	return 0;
}
/*
mpiCC -g -Wall mpi_hello.cpp  -o m
mpirun -np 2 ./m
*/