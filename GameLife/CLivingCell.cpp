//
// Created by riv on 10.09.16.
//

#include "CLivingCell.h"

CLivingCell::CLivingCell(){
    isPopulatedNow = bool(std::rand() % 2);
}

void CLivingCell::settle(){
    isPopulatedNow = true;
}

void CLivingCell::exterminate(){
    isPopulatedNow = false;
}

bool CLivingCell::isPopulated(){
    return isPopulatedNow;
}