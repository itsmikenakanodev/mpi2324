#include <iostream>
#include <mpi.h>

int main(int argc, char** argv){
    MPI_Init(&argc,&argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if(rank == 0){
         MPI_Send(&rank //referencia al vector de elementos a enviar
    		,1 // tamaño del vector a enviar
    		,MPI_INT // Tipo de dato que envias
    		,rank+1 // pid del proceso destino
    		,0 //etiqueta
    		,MPI_COMM_WORLD); //Comunicador por el que se manda
        
    }

    std::printf("Rank %d of %d procs\n",rank,nprocs);

    MPI_Finalize();

    return 0;
}