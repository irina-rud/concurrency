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
    int length;
    int height;
public:

    std::vector<int> torus;

    CTorus(int len, int heig);

    int getHeight();

    int getLength();

    int isLiving(int x, int y);

    int numOfNeighbours(int x, int y);

    void action(int act, int wasHere, int x, int y);

    int countCell( int x, int y);

    void print();

    void set_nothing(int x, int y);

    void set(std::vector<int>& input);

    int getCell(int x, int y);

    void setUpperBound(std::vector<int>& bound);

    void setLowerBound(std::vector<int>& bound);

    void lowerBound(std::vector<int>& result);

    void upperBound(std::vector<int>& result);

    void getTorus(std::vector<int>& result);
};


