#include <iostream>
#include <mpi.h>

int main(int argc, char** argv){
    std::cout << "HI!" <<  std::endl;

    int rank;
    int size;

    int MESSAGE = 101;

    MPI_Status status;

    int st = MPI_Init(&argc, &argv);
    if (st != 0) {
        std::cout << "MPI error\n";
        return -1;
    }


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0){
        std::cout << "rank is:" << rank << "size is:" << size<< std::endl;
    }

   MPI_Finalize();

}