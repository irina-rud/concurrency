//
// Created by riv on 10.09.16.
//

#pragma once
#include <iostream>
#include <vector>


#define SETTLE 0
#define  EXTERMINATE 1
#define NOTHING 2

#define notchanged 0
#define settled 1
#define empty (-1)


class CTorus {
private:
    std::vector<int> torus;
    long length;
    long height;
public:
    CTorus(long len, long heig);

    long getHeight();

    long getLength();

    int isLiving(long x, long y);

    int numOfNeighbours(long x, long y);

    void action(int act, int wasHere, long x, long y);

    int countCell( long x, long y);

    void print();

    void set_nothing(long x, long y);

    void set(std::vector<int>& input);

    int getCell(long x, long y);
};


