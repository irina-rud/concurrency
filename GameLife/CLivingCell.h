//
// Created by riv on 10.09.16.
//
#pragma once

#include <iostream>
#include <ctime>
#include <cstdlib>

class CLivingCell {
private:
    bool isPopulatedNow;
public:
    CLivingCell();

    void settle();

    void exterminate();

    bool isPopulated();

    ~CLivingCell(){}

};


