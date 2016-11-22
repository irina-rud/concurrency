//
// Created by riv on 10.09.16.
//

#pragma once
#include "CLivingCell.h"
#include <iostream>


#define SETTLE 0
#define  EXTERMINATE 1
#define NOTHING 2

class CTorus {
private:
    std::vector< std::vector<CLivingCell> > torus;
    long length;
    long height;
public:
    CTorus(long len, long heig);

    long getHeight();

    long getLength();

    bool isLiving(long x, long y);

    int numOfNeighbours(long x, long y);

    void action(int act, long x, long y);

    int countCell( long x, long y);

    void print();
};


