#include <iostream>
#include <mpi.h>
#include <vector>

#define TAMANIO 10

int sumar(int* tmp,int n){
    int suma=0;
    for(int i=0;i<n;i++){
        suma +=tmp[i];
    }
    return suma;
}
//comunicacion sincrona
int main(int argc, char** argv){
    MPI_Init(&argc,&argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);


    int block_size=0;
    int residuo=0;

    if(TAMANIO%nprocs==0){
        block_size=TAMANIO/nprocs;  // 8/4=2
    }else{
        residuo=TAMANIO%(nprocs-1);  //10%3=1; r0 2+2; r1
        block_size=(TAMANIO-residuo)/(nprocs-1);  //10-1 /3 = 3
    }

    if(rank == 0){
        int data[TAMANIO]; //vector de 100 elemento sin inicializar
        std::printf("total ranks:%d\n",nprocs);

        for(int i=0;i<TAMANIO;i++){
            data[i]=i;
        }

        std::printf("block size %d y residuo %d \n",block_size,residuo);
        for(int rank_id=0; rank_id<(nprocs-1); rank_id++){
            std::printf("Rank_0 Enviando Datos a RANK_%d\n",rank_id);
            int start=(rank_id*block_size)+residuo;  //   1*2 +0   data[2]   caso contrario  d[4]   r0=data[0]  r1=0*3+1=data[1]  r2=1*3+1=d[4] r3=2*3+1=d[7]
            MPI_Send(&data[start],block_size,MPI_INT,(rank_id+1),0,MPI_COMM_WORLD);
        }

        int suma_ranks[nprocs];
        suma_ranks[0] = sumar(data,residuo);

        for(int rank_id=1; rank_id<nprocs; rank_id++){
            MPI_Recv(&suma_ranks[rank_id],1,MPI_INT,rank_id,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }

        std::printf("sumas parciales: %d, %d, %d, %d \n",suma_ranks[0],suma_ranks[1],suma_ranks[2],suma_ranks[3]);

        int suma_total= sumar(suma_ranks,nprocs);
        std::printf("suma total: %d \n",suma_total);

    }else{
        int datos[block_size];
        std::printf("blocksize %d \n",block_size);
        MPI_Recv(datos,block_size,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        std::string str="";
        int suma_parcial =sumar(datos,block_size);

        std::printf("Rank_%d enviando suma parcial %d \n",rank,suma_parcial);
        MPI_Send(&suma_parcial,1,MPI_INT,0,0,MPI_COMM_WORLD);

        /*for(int i=0;i<25;i++){
            str=str+std::to_string(data[i])+",";
        }
        std::printf("Rank_%d datos recibidos===>%s \n",rank,str.c_str());*/
    }

    //std::printf("Rank %d of %d procs\n",rank,nprocs);

    MPI_Finalize();

    return 0;
}

