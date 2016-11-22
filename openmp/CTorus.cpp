//
// Created by riv on 10.09.16.
//

#include "CTorus.h"


CTorus::CTorus(long len, long heig):length(len), height(heig){
    torus.resize(length*height,  notchanged);
}

void CTorus::set(std::vector<int>& input){
    torus = input;
}

long CTorus::getHeight(){
    return height;
}

long CTorus::getLength(){
    return length;
}

void CTorus::print(){
    std::cout << "Torus: " << std::endl;
    for (int i = 0; i< height; ++i){
        for (int j = 0; j < length; ++j){
            int inside = torus[i*length + j];
            if (inside == settled){
                std::cout << "*";
            } else if (inside == empty){
                std::cout<< "-";
            } else{
                std::cout << "?";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "end." << std::endl << std::endl;
}


int CTorus::isLiving(long x, long y){
    if (x < 0){
        x = (x + length) % length;
    } else{
        x = x % length;
    }
    if (y < 0){
        y = (y + height) % height;
    } else{
        y = y % height;
    }
    return torus[y * length + x];
}

int CTorus::numOfNeighbours(long x, long y){
    std::vector< std::vector< int > > steps= {{-1, -1}, {0, -1}, {1, -1},
                                              {-1, 0},           {1, 0},
                                              {-1, 1},  {0, 1},  {1, 1}};
    int res = 0;
    for (auto pair: steps){
        if ((isLiving(x + pair[0], y + pair[1])) == settled){
            res++;
        }
    }
    return res;
}

void CTorus::action(int act, int wasHere, long x, long y){
    if (act == EXTERMINATE){
        torus[y * length + x] = empty;
    }else if (act == SETTLE){
        torus[y * length + x] = settled;
    }else{
        torus[y * length + x] = wasHere;
    }
}

int CTorus::countCell( long x, long y){
    int numOfNeighb  = numOfNeighbours(x,y);
    if (numOfNeighb == 3 && (torus[y * length + x] == empty) ){
        return SETTLE;
    }else if(( numOfNeighb > 3 || numOfNeighb < 2) && (torus[y * length + x] == settled) ){
        return EXTERMINATE;
    }else{
        return NOTHING;
    }
}

void CTorus::set_nothing(long x, long y) {
    torus[y * length + x] = notchanged;
}

int CTorus::getCell(long x, long y) {
    return torus[y * length + x];
}



