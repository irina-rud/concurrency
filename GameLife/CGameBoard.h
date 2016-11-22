//
// Created by riv on 10.09.16.
//

#pragma once

#include <iostream>
#include <vector>
#include <exception>
#include "CLivingCell.h"
#include "CTorus.h"

class CGameBoard {
private:
    std::vector< CTorus > twoToruses;
    int now;
public:
    CGameBoard();

    void changeMode();

    void countCell( long x, long y);

    void printTorus();

    ~CGameBoard();
};

