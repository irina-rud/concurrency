//
// Created by riv on 10.09.16.
//

#include "CGameBoard.h"

//Объявляем три торы. на now-считаем, на now+next - пишем notchanged, на now+prev смотрим, что было
CGameBoard::CGameBoard(int len, int heig):length(len), height(heig){
    now = 0;
    if (heig < 0 || len < 0)
        throw new std::exception();
    else{
        threeToruses.push_back(CTorus(len,heig));
        threeToruses.push_back(CTorus(len,heig));
        threeToruses.push_back(CTorus(len,heig));
    }
}


void CGameBoard::changeMode(){
    now = (now + next) % 3;
}

void CGameBoard::setNewNow(std::vector<int>& input){
    threeToruses[(now+prev)%3].set(input);
}


int CGameBoard::getHeight(){
    return height;
}

int CGameBoard::getLength(){
    return length;
}

void CGameBoard::countCell( int index) {
    if (index < 0){
        index += length*height;
    }
    index = index % (length*height);
    int x = index % length;
    int y = index / length;
    int act  = threeToruses[(now + prev) % 3].countCell( x, y);
    threeToruses[now].action(act, threeToruses[(now + prev) % 3].getCell( x, y), x,y);
    threeToruses[(now + next) % 3].set_nothing(x, y);
}


void CGameBoard::printNextTorus(){
    threeToruses[(now+next)%3].print();
}

void CGameBoard::printPrevTorus(){
    threeToruses[(now+prev)%3].print();
}

void CGameBoard::printTorus(){
    threeToruses[now].print();
}

CGameBoard::~CGameBoard(){
    std::cout << "Have a nice day";
}

bool CGameBoard::isCounted(int index) {
    if (index < 0){
        index += length*height;
    }
    index = index % (length*height);
    int x = index % length;
    int y = index / length;
    return !(threeToruses[now].getCell(x,y) == notchanged);
}

int CGameBoard::getCell(int index) {
    int x = index % length;
    int y = index / length;
    return threeToruses[now].getCell(x,y);
}

int CGameBoard::getSize() {
    return length*height;
}

int CGameBoard::getNow() {
    return now;
}


void CGameBoard::setUpperBound(std::vector<int>& bound){
    threeToruses[now].setUpperBound(bound);
}

void CGameBoard::setLowerBound(std::vector<int>& bound){
    threeToruses[now].setLowerBound(bound);
}

std::vector<int> CGameBoard::lowerBound(){
    threeToruses[now].lowerBound();
}

std::vector<int> CGameBoard::upperBound(){
    threeToruses[now].upperBound();
}

std::vector<int> CGameBoard::getTorus(){
    return threeToruses[(now + prev) % 3].torus;
}





