#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "CGameBoard.h"
#include "CTorus.h"



int main(){
    std::ifstream F("/home/riv/concurrency/openmp/input.txt");

    long kernels;
    long iterations;
    long higth;
    long length;

    F >> kernels >> iterations >> higth >> length;
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



    int nthreads, tid;

    CGameBoard board(length,higth);
    board.setNewNow(torus);
    long size = length * higth;

    #pragma omp parallel num_threads(kernels)
    #pragma omp parallel private(nthreads, tid)
    for (long i = 0; i < iterations; ++i){
        #pragma omp for
        for (long j = 0; j < size; ++j){
            board.countCell(j);
        }
        if (tid == 0)
        {
            board.changeMode();
        }

        #pragma omp barrier

    }

    board.printPrevTorus();

//    #pragma omp parallel num_threads(10)
//        #pragma omp for
//            for(int n=0; n<100; ++n) printf(" %d", n);

}