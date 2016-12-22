//
// Created by riv on 10.09.16.
//

#pragma once

#include <iostream>
#include <vector>
#include "CTorus.h"

#define next 1
#define prev 2

class CGameBoard {
private:
    int length;
    int height;
    std::vector< CTorus > threeToruses;
    int now;
public:
    CGameBoard(int len, int heig);

    void changeMode();

    void countCell( int index);

    void printTorus();

    int getHeight();

    int getLength();

    ~CGameBoard();

    void setNewNow(std::vector<int>& input);

    bool isCounted(int index);

    int getCell(int index);

    int getSize();

    void printNextTorus();

    void printPrevTorus();

    int getNow();

    void setUpperBound(std::vector<int>& bound);

    void setLowerBound(std::vector<int>& bound);


    void upperBound(std::vector<int>& result);

    void lowerBound(std::vector<int>& result);

    void getTorus(std::vector<int> &res);
};

