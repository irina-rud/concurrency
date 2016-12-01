#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <omp.h>
#include "CGameBoard.h"



int main(){
    //std::ifstream F("/home/riv/concurrency/openmp/input.txt");
    std::ifstream F("/home/riv/concurrency/openmp/input_rand.txt");

    long kernels;
    long iterations;
    long higth;
    long length;

    F >> kernels >> iterations >> higth >> length;
    std::vector<int> torus;
    std::string line;
    //std::cout <<kernels<< " - "  << iterations<< " - "  << higth << " - " << length;
    srand (time(NULL));
    for (int i = 0; i < higth; ++i){
        for (int j = 0; j < length; ++j){
            int node = rand()%2;
            if (node == 0){
                torus.push_back(settled);
            } else if (node == 1 ){
                torus.push_back(empty);
            } else{
                throw new std::exception();
            }
        }
    }
    F.close();

    CGameBoard board(length,higth);
    board.setNewNow(torus);
//
//    for (int i = 0; i < higth; ++i){
//        F >> line;
//        if (line.size() < length){
//            throw new std::exception();
//        }
//        for (int j = 0; j < length; ++j){
//            if (line[j] == '*'){
//                torus.push_back(settled);
//            } else if (line[j] == '-' ){
//                torus.push_back(empty);
//            } else{
//                throw new std::exception();
//            }
//        }
//    }
//    F.close();
//
//
//    CGameBoard board(length,higth);
//    board.setNewNow(torus);


    CGameBoard* board_link = & board;
    long all_size = length * higth;
    long size = length * higth / (kernels);
    long rest = length * higth - size*kernels;

    //std::cout << size<< " - " <<rest <<std::endl;
    time_t start = time(NULL);
    #pragma omp parallel num_threads(kernels)
    {
        size_t start;
        int name = omp_get_thread_num();
        if (name < rest) {
            start = (size_t) ((size + 1) * name);
            //std::cout << name <<" start " << start << std::endl;
        } else {
            start = (size_t) (size * name + rest);
            //std::cout << name <<" start " << start << std::endl;
        }
        for (int j = 0; j < iterations; ++j) {
//            std::cout<< " It's iteration "<< j << " my name " << name <<std::endl;
            {
                for (int i = 0; !(board_link->isCounted(start + i)); ++i) {
                    //std::cout << "my name:" << name << " I'm going right "<< i <<std::endl;
                    board_link->countCell(start + i);
                }
                for (int i = -1; !(board_link->isCounted(start + i)); --i) {
                    //std::cout << "my name:" << name << " I'm going left "<< i <<std::endl;
                    board_link->countCell(start + i);
                }
                //std::cout << "my name:" << name << " I've done " <<std::endl;
            }
            #pragma omp barrier
            if (name == 0) {
                board_link->changeMode();
            }
            #pragma omp barrier
        }
    }
    time_t end = time(NULL);
    std::cout << "Execution Time: " << (double)(end-start) << " Seconds" <<std::endl;
    return 0;
}