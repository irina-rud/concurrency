#include <iostream>
#include <thread>
#include <fstream>
#include "CGameBoard.h"
#include <time.h>
#include <stdlib.h>


using namespace std;

void worker(int name, size_t start,CGameBoard& board, vector<int>& swamp, size_t howLong){
    //std::cout << "my name:" << name << std::endl;
    for (int j = 0; j < howLong; ++j){
        //std::cout << "my name:" << name << " It's iteration "<< j <<std::endl;
        for (int i= 0; !(board.isCounted(start+i)); ++i){
            //std::cout << "my name:" << name << " I'm going right "<< i <<std::endl;
            board.countCell(start+i);
        }
//        board.printPrevTorus();
//        board.printTorus();
//        board.printNextTorus();
        for (int i= -1; !(board.isCounted(start+i)) ; --i){
            //std::cout << "my name:" << name << " I'm going left "<< i <<std::endl;
            board.countCell(start+i);
        }
        //std::cout << "my name:" << name << " I've done " <<std::endl;
        ++swamp[name];
        //std::cout << "my name:" << name << " Swamp now " << swamp[name] << std::endl;
        while(board.isCounted(start)){
            //std::cout << "my name:" << name << std::endl;
        }
    }
}

void master(vector<int>& swamp, CGameBoard& board, size_t howLong, long workersSize){
    //std::cout << "I'm a boss"<< std::endl;
    for  (int i = 0; i < howLong; ++i){
        //std::cout << "I'm a boss" << " It's iteration"<< i<< std::endl;
//        board.printPrevTorus();
//        board.printTorus();
//        board.printNextTorus();

        long sum = 0;
        while (sum < (i+1)*workersSize){
            sum = 0;
            for (int el : swamp){
                sum += el;
            }
            //std::cout << "I'm a boss, sum is " << sum << std::endl;
        }
        //std::cout << "I'm a boss, sum is " << sum << std::endl;
//        board.printPrevTorus();
//        board.printTorus();
//        board.printNextTorus();
        //std::cout << "I'm a boss, now is " << board.getNow() <<std::endl;
        board.changeMode();
    }
}


int main() {
    std::ifstream F("/home/riv/concurrency/Game1/input_rand.txt");
    std::ofstream O("/home/riv/concurrency/Game1/output.txt");

    long kernels;
    long iterations;
    long higth;
    long length;

    F >> kernels >> iterations >> higth >> length;
    std::vector<int> torus;
    std::string line;

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
    std::vector<int> swamp;
    swamp.resize((kernels -1), 0);
    std::vector<std::thread> threads;
    std::thread masterTread(master, std::ref(swamp), std::ref(board), iterations, (kernels - 1));

    long size = length * higth / (kernels-1);
    long rest = length * higth - size*(kernels-1);


    time_t start = time(NULL);
    for (int i  = 0; i < kernels-1; ++i){
        if (i < rest){
            std::thread thr(worker,i, (size_t)((size+1)*i), std::ref(board), std::ref(swamp), iterations);
            threads.emplace_back(std::move(thr));
        } else {
            std::thread thr(worker,i, (size_t)(size*i + rest), std::ref(board), std::ref(swamp), iterations);
            threads.emplace_back(std::move(thr));
        }
    }

    masterTread.join();
    for (int i = 0; i < threads.size();++i){
        threads[i].join();
    }
    time_t end = time(NULL);
    std::cout<<"Execution Time: "<< (double)(end-start)<<" Seconds"<<std::endl;
    return 0;
}