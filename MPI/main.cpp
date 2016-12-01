#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "CGameBoard.h"


CGameBoard& masterTr(int my_rank, int num_workers, int one_block_hight, int length ){

    std::vector<MPI_Request> reqs(num_workers);
    std::vector<MPI_Status> stats(num_workers * 2);
    MPI_Status status;

    srand (time(NULL));

    std::vector<std::vector<int>> parts;
    parts.resize(num_workers);

    for (int rank = 0 ; rank <num_workers; ++rank){
        parts[rank].resize((2 + one_block_hight) * length);

        for (int i = 1; i < one_block_hight - 1; ++i){
            for (int j = 0; j < length; ++j){
                int node = rand()%2;
                if (node == 0){
                    parts[rank].push_back(settled);
                } else if (node == 1 ){
                    parts[rank].push_back(empty);
                } else{
                    throw new std::exception();
                }
            }
        }
    }
    for (int i = 0; i < num_workers; ++i){
        MPI_Isend(parts[i].data(), parts[i].size(), MPI_INT, (i+1), 0, MPI_COMM_WORLD, &reqs[i ]);
    }
    MPI_Waitall(num_workers, reqs.data(), stats.data());

    for (int i = 0; i < num_workers; i++) {
        MPI_Recv(parts[i].data(), parts[i].size(),
                 MPI_INT, i, 0, MPI_COMM_WORLD, &status);
    }

    CGameBoard gameBoard(one_block_hight * num_workers, length);
    std::vector<int> result;
    for (auto p: parts){
        result.insert(result.end(),p.begin(),p.end());
    }
    gameBoard.setNewNow(result);
    return gameBoard;

}

void worker(int my_rank, int num_workers, int block_hight, int length, int iterations){
    MPI_Status status;
    std::vector<int> my_part;
    my_part.resize(length * (block_hight + 2));
    std::vector<int> new_part;
    new_part.resize(length * (block_hight));
    MPI_Recv(new_part.data(), 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

    for (int i = 0; i < length; ++i){
        my_part.push_back(notchanged);
    }
    my_part.insert(my_part.end(), new_part.begin(),new_part.end());
    for (int i = 0; i < length; ++i){
        my_part.push_back(notchanged);
    }
    int left_rank = (my_rank - 1 + num_workers)% num_workers;
    int right_rank = (my_rank + 1)% num_workers;
    CGameBoard board(block_hight + 2, length);
    board.setNewNow(my_part);

    std::vector<int> send_left_line, send_right_line;
    std::vector<int> recv_left_line, recv_right_line;

    send_left_line.resize(length);
    send_right_line.resize(length);
    recv_left_line.resize(length);
    recv_right_line.resize(length);


    MPI_Request reqs[4];
    MPI_Status stats[4];

    for (int i = 0; i < iterations; ++i ){

        send_left_line = board.upperBound();
        send_right_line = board.lowerBound();

        MPI_Isend(send_left_line.data(), send_left_line.size(), MPI_INT, left_rank, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Isend(send_right_line.data(), send_left_line.size(), MPI_INT, right_rank, 1, MPI_COMM_WORLD, &reqs[1]);
        MPI_Irecv(recv_left_line.data(), recv_left_line.size(), MPI_INT, left_rank, 1, MPI_COMM_WORLD, &reqs[2]);
        MPI_Irecv(recv_right_line.data(), recv_right_line.size(), MPI_INT, right_rank, 0, MPI_COMM_WORLD, &reqs[3]);
        MPI_Waitall(4, reqs, stats);

        board.setLowerBound(recv_right_line);
        board.setUpperBound(recv_left_line);
        board.changeMode();
        for (int y = 1; y <= block_hight; ++y){
            for (int x = 0; x < length; ++x){
                board.countCell(x + length*y);
            }
        }
    }
    new_part.insert(new_part.begin(), board.getTorus().begin() + length,board.getTorus().end() - length );
    MPI_Send(new_part.data(), new_part.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
}


int main(int argc, char** argv){
    std::cout << "HI!" <<  std::endl;

    int iterations = 100, higth = 100, length = 1000;

    MPI_Status status;
    int st = MPI_Init(&argc, &argv);
    if (st != 0) {
        std::cout << "MPI error\n";
        return -1;
    }

    int rank, num_nodes;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_nodes);

    if (rank == 0){
        masterTr(0,num_nodes - 1, higth, length).printTorus();
    } else{
        worker(rank, num_nodes - 1, higth, length, iterations);
    }

    MPI_Finalize();

}