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
    long length;
    long height;
    std::vector< CTorus > threeToruses;
    int now;
public:
    CGameBoard(long len, long heig);

    void changeMode();

    void countCell( long index);

    void printTorus();

    long getHeight();

    long getLength();

    ~CGameBoard();

    void setNewNow(std::vector<int>& input);

    bool isCounted(long index);

    int getCell(long index);

    long getSize();

    void printNextTorus();

    void printPrevTorus();

    int getNow();

    void setUpperBound(std::vector<int>& bound);

    void setLowerBound(std::vector<int>& bound);

    std::vector<int> lowerBound();

    std::vector<int> upperBound();

    std::vector<int> getTorus();
};

