#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include "CGameBoard.h"


void masterTr(int my_rank, int num_workers, int one_block_hight, int length, CGameBoard* result_board) {

    std::vector<MPI_Request> reqs(num_workers);
    std::vector<MPI_Status> stats(num_workers);
    MPI_Status status;

    srand(time(NULL));

    std::vector<std::vector<int> > parts;
    parts.resize(size_t(num_workers));

    for (int rank = 0; rank < num_workers; ++rank) {
        parts[rank].reserve(size_t(one_block_hight * length));

        for (int i = 1; i < one_block_hight + 1; ++i) {
            for (int j = 0; j < length; ++j) {
                int node = rand() % 2;
                if (node == 0) {
                    parts[rank].push_back(settled);
                } else if (node == 1) {
                    parts[rank].push_back(empty);
                } else {
                    throw new std::exception();
                }
            }
        }
    }


    for (int i = 0; i < num_workers; ++i) {
        MPI_Isend(parts[i].data(), int(parts[i].size()), MPI_INT, (i + 1), 0, MPI_COMM_WORLD, &reqs[i]);
    }

    MPI_Waitall(num_workers, reqs.data(), stats.data());

    for (int i = 0; i < num_workers; i++) {
        //std::cout << "reciving from  " << i + 1 <<  std::endl;
        MPI_Recv(parts[i].data(), int(parts[i].size()),
                 MPI_INT, (i+1), 0, MPI_COMM_WORLD, &status);
        //std::cout << "recived " << i <<  std::endl;

    }

    std::vector<int> result;
    //std::cout << "recived" << std::endl;
    result.reserve(num_workers*one_block_hight*length);
    for (int i = 0 ; i < parts.size(); ++i) {
        result.insert(result.end(), parts[i].begin(), parts[i].end());
    }

//    result_board->setNewNow(result);
//    result_board->printPrevTorus();
}

void worker(int my_rank, int num_workers, int block_hight, int length, int iterations) {
    MPI_Status status;
    std::vector<int> my_part;
    my_part.resize(size_t(length * (block_hight + 2)));

    for (int i = 0; i < length *(block_hight + 2) ; ++i){
        my_part[i] = notchanged;
    }

    std::vector<int> data;
    data.resize(length * block_hight);

    MPI_Recv(data.data(),int(length * block_hight), MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

    my_part.insert(my_part.begin()+length, data.begin(), data.end());

    int left_rank = (my_rank - 1 ) % (num_workers + 1);
    if (left_rank == 0){
        left_rank = num_workers;
    }
    int right_rank = (my_rank + 1) % (num_workers +1);
    if (right_rank == 0){
        right_rank = 1;
    }

    //std::cout <<"left:"<<left_rank<< " me:" <<my_rank <<" right:" << right_rank <<std::endl;
    CGameBoard board(length,block_hight + 2);
    board.setNewNow(my_part);
    //board.printPrevTorus();

    std::vector<int> send_left_line, send_right_line;
    std::vector<int> recv_left_line, recv_right_line;

    send_left_line.reserve(length);
    send_right_line.reserve(length);
    recv_left_line.resize(length);
    recv_right_line.resize(length);

    MPI_Request reqs[4];
    MPI_Status stats[4];

    for (int i = 0; i < iterations; ++i) {
        send_left_line.clear();
        send_right_line.clear();
        recv_left_line.clear();
        recv_right_line.clear();

        send_left_line.reserve(length);
        send_right_line.reserve(length);
        recv_left_line.resize(length);
        recv_right_line.resize(length);

        board.upperBound(send_left_line);
        board.lowerBound(send_right_line);

//        for (int i = 0; i < length; ++i){
//            std::cout << send_right_line[i] <<' ';
//        }
//        std::cout << std::endl;

        MPI_Sendrecv(send_left_line.data(), send_left_line.size(), MPI_INT, left_rank, 0,
                     recv_right_line.data(), recv_right_line.size(), MPI_INT, right_rank, 0,
                     MPI_COMM_WORLD, &stats[0]);
        //std::cout << "done1" << std::endl;

        MPI_Sendrecv(send_right_line.data(), send_left_line.size(), MPI_INT, right_rank, 0,
                     recv_left_line.data(), recv_left_line.size(), MPI_INT, left_rank, 0,
                     MPI_COMM_WORLD, &stats[1]);

        //std::cout << "done" << std::endl;
        board.setLowerBound(recv_right_line);
        board.setUpperBound(recv_left_line);

        //board.printPrevTorus();

        for (int y = 1; y <= block_hight; ++y) {
            for (int x = 0; x < length; ++x) {
                board.countCell(x + length * y);
            }
        }
        board.changeMode();
    }
//    board.printPrevTorus();
    std::vector<int> res_part;
    res_part.reserve(size_t(length * block_hight));
    board.getTorus(res_part);
//    for (int i = 0; i < res_part.size(); ++i){
//            std::cout << res_part[i] <<' ';
//    }
//    std::cout << std::endl;
    MPI_Send(res_part.data(), res_part.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
}


int main(int argc, char **argv) {
    int heigth_all = 10000, kernels = 30
    ;

    int iterations = 200, length = 100;

    int higth = (int)(heigth_all / kernels);

    MPI_Status status;
    int st = MPI_Init(&argc, &argv);
    if (st != 0) {
        std::cout << "MPI error\n";
        return -1;
    }
    int rank, num_nodes;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_nodes);

    CGameBoard result(length,(num_nodes-1)*higth);
    if (rank != 0) {
        worker(rank, num_nodes - 1, higth, length, iterations);
    } else {
        double t0 = MPI_Wtime();
        masterTr(0, num_nodes - 1, higth, length, &result);
        printf("Time: %lf\n", MPI_Wtime() - t0);
//        result.printTorus();
    }

    MPI_Finalize();

}