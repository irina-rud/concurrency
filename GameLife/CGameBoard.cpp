//
// Created by riv on 10.09.16.
//

#include "CGameBoard.h"

CGameBoard::CGameBoard(){
    now = 0;
    long length;
    long height;
    std::cout << "Length of the torus: ";
    std::cin >> length;
    std::cout << std::endl << "Height of the torus: ";
    std::cin >> height;
    if (height < 0 || length < 0)
        throw new std::exception();
    else{
        twoToruses.push_back(CTorus(length,height));
        twoToruses.push_back(CTorus(length,height));
    }
}

void CGameBoard::changeMode(){
    now = (now + 1) % 2;
}




void CGameBoard::countCell( long x, long y){
    int act  = twoToruses[now].countCell( x, y);
    twoToruses[(now+1) % 2].action(act, x,y);

}

void CGameBoard::printTorus(){
    twoToruses[now].print();
}

~CGameBoard::CGameBoard(){
    std::cout << "Have a nice day";
}
