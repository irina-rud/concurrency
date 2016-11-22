//
// Created by riv on 10.09.16.
//

#include "CTorus.h"

CTorus::CTorus(long len, long heig):length(len), height(heig){
    srand(time(0));
    torus.resize(height);
    for (long i = 0; i < height; ++i){
        for (long j = 0; j < length; ++j){
            torus[i].push_back(CLivingCell());
        }
    }
}

long CTorus::getHeight(){
    return height;
}

long CTorus::getLength(){
    return length;
}

bool CTorus::isLiving(long x, long y){
    if (x < 0){
        x = (x % length) + length;
    } else{
        x = x % length;
    }
    if (y < 0){
        y = (y % height) + height;
    } else{
        y = y % height;
    }
    return torus[y][x].isPopulated();
}

int CTorus::numOfNeighbours(long x, long y){
    std::vector< std::vector< int > > steps= {{-1, -1}, {0, -1}, {1, -1},
                                              {-1, 0},           {1, 0},
                                              {-1, 1},  {0, 1},  {1, 1}};
    int res = 0;
    for (auto pair: steps){
        if (isLiving(x + pair[0], y + pair[1])){
            res++;
        }
    }
    return res;
}

void CTorus::action(int act, long x, long y){
    if (act == EXTERMINATE){
        torus[y][x].exterminate();
    }
    if (act == SETTLE){
        torus[y][x].settle();
    }
}

int CTorus::countCell( long x, long y){
    int numOfNeighb  = numOfNeighbours(x,y);
    if (numOfNeighb == 3 && !(torus[y][x].isPopulated()) ){
        return SETTLE;
    }

    if (( numOfNeighb > 3 || numOfNeighb < 2) && (torus[y][x].isPopulated()) ){
        return EXTERMINATE;
    }

    return NOTHING;
}

void CTorus::print(){
    std::cout << "Torus: " <<endl;
    for (auto line:torus){
        for (auto cell:line){
            if (cell.isPopulated()){
                cout << "*";
            } else{
                cout<< "-";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "end." << std::endl << std::endl;
}