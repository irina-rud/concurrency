#include <iostream>
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <fstream>
#include <vector>
#include "CGameBoard.h"
#include "CTorus.h"
#define TagUpper 231
#define TagLower 221
#define TagPart 578


int main(int argc, char** argv){
    std::ifstream F("input.txt");

    long iterations;
    long higth;
    long length;

    int MESSAGE = 101;

    F >> iterations >> higth >> length;
    std::vector<int> torus;
    std::string line;

    for (int i = 0; i < higth; ++i){
        F >> line;
        if (line.size() < length){
            throw new std::exception();
        }
        for (int j = 0; j < length; ++j){
            if (line[j] == '*'){
                torus.push_back(settled);
            } else if (line[j] == '-' ){
                torus.push_back(empty);
            } else{
                throw new std::exception();
            }
        }
    }
    F.close();

    MPI_Status status;

    int st = MPI_Init(&argc, &argv);
    if (st != 0) {
        std::cout << "MPI error\n";
        return -1;
    }


    long rank;
    long size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long bounds[2];
    if (higth % size < rank){
        bounds[0] = higth % size + rank*long(higth/size)+1;
        bounds[1] =  higth % size + (rank + 1)*long(higth/size) ;
    }else{
        bounds[0] = long(higth/size) * rank + rank;
        bounds[1] =  long(higth/size) * (rank + 1) + rank;
    }


    long localHigth = bounds[1] - bounds[0] + 3;
    CGameBoard board(length,localHigth);

    std::vector<int> localTorus;
    localTorus.resize(localHigth*length);
    long pr = (bounds[0] + higth - 1) % higth;
    for (int j = 0; j < length; ++j){
        localTorus.push_back(torus[pr*length + j]);
    }
    for (int i = bounds[0]  % higth; i < bounds[1]; ++i){
        for (int j = 0; j < length; ++j){
            localTorus.push_back(torus[i*length + j]);
        }
    }
    long ls = (bounds[1] + 1) % higth;
    for (int j = 0; j < length; ++j){
        localTorus.push_back(torus[ls*length + j]);
    }
    board.setNewNow(localTorus);
    long thorusSize = length * localHigth;

    std::vector<int> intUpperBound;
    std::vector<int> intLowerBound;
    std::vector<int> myUpperBound;
    std::vector<int> myLowerBound;

    intUpperBound.resize(length);
    intLowerBound.resize(length);
    myUpperBound.resize(length);
    myLowerBound.resize(length);


    for (long i = 0; i < iterations; ++i){
        for (long j = length; j < thorusSize - 2* length; ++j){
            board.countCell(j);
        }
        myLowerBound = board.lowerBound();
        myUpperBound = board.upperBound();
        MPI_Sendrecv(&myLowerBound[0], length, MPI_INT,(rank+1)%size,TagLower,

                         &intUpperBound[0], length, MPI_INT,(rank+size-1)%size,TagLower,

                         MPI_COMM_WORLD, &status);


        MPI_Sendrecv(&myUpperBound[0], length, MPI_INT,(rank+size-1)%size,TagUpper,

                     &intLowerBound[0], length, MPI_INT,(rank+1)%size, TagUpper,

                     MPI_COMM_WORLD, &MESSAGE);
        board.setLowerBound(intLowerBound);
        board.setUpperBound(intUpperBound);

        board.changeMode();
    }
    if (rank == 0){
        std::vector<int> resTorus;
        std::vector<int> partTorus;
        partTorus.resize((long(higth/size) + 1)*length);
        partTorus = board.getTorus();
        for (long j = length; j < (localHigth - 1) * length; ++j){
            resTorus.push_back(partTorus[j]);
        }

        torus.resize(length*higth);
        long localBounds[2];

        long localH;

        for (long i = 1; i < size; ++i){
            if (higth % size < i){
                localBounds[0] = higth % size + i*long(higth/size)+1;
                localBounds[1] =  higth % size + (i + 1)*long(higth/size) ;
            }else{
                localBounds[0] = long(higth/size) * i + i;
                localBounds[1] =  long(higth/size) * (i + 1) + i;
            }
            localH = localBounds[1] - localBounds[0] + 1;
            MPI_Recv(&partTorus[0], localH * length, i, TagPart);

            for (long j = 0; j < localH * length; ++j){
                resTorus.push_back(partTorus[j]);
            }
        }

        CGameBoard result(length, higth);
        result.setNewNow(resTorus);
        result.printTorus();
    } else{
        std::vector<int> partTorus;
        partTorus.resize(localHigth * length);
        partTorus = board.getTorus();
        std::vector<int> toSendTorus;
        for (long j = length; j < (localHigth - 1) * length; ++j){
            toSendTorus.push_back(partTorus[j]);
        }
        MPI_Send(&toSendTorus[0],localHigth * length, 0, TagPart );
    }

    MPI_Finalize();

}