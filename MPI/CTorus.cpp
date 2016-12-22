//
// Created by riv on 10.09.16.
//

#include "CTorus.h"


CTorus::CTorus(int len, int heig) : length(len), height(heig) {
    torus.resize(length * height, notchanged);
}

void CTorus::set(std::vector<int> &input) {
    torus = input;
}

int CTorus::getHeight() {
    return height;
}

int CTorus::getLength() {
    return length;
}

void CTorus::print() {
    std::cout << "Torus: " << std::endl;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < length; ++j) {
            int inside = torus[i * length + j];
            if (inside == settled) {
                std::cout << "*";
            } else if (inside == empty) {
                std::cout << "-";
            } else {
                std::cout << "?";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "end." << std::endl << std::endl;
}

void CTorus::upperBound(std::vector<int> &result) {
    std::vector<int>::const_iterator first = torus.begin() + length;
    std::vector<int>::const_iterator last = torus.begin() + 2 * length;
    result.insert(result.begin(), first, last);

}


void CTorus::setUpperBound(std::vector<int> &bound) {
    for (int i = 0; i < length; ++i) {
        torus[i] = bound[i];
    }
}


void CTorus::setLowerBound(std::vector<int> &bound) {
    for (int i = 0; i < length; ++i) {
        torus[(height - 1) * length + i] = bound[i];
    }
}

void CTorus::lowerBound(std::vector<int> &result) {
    std::vector<int>::const_iterator first = torus.begin() + (height - 2) * length;
    std::vector<int>::const_iterator last = torus.begin() + length *(height - 1);
    result.insert(result.begin(), first, last);
}

void CTorus::getTorus(std::vector<int> &result) {
    std::vector<int>::const_iterator first = torus.begin() + length;
    std::vector<int>::const_iterator last = torus.begin() + length *(height - 1);
    result.insert(result.begin(), first, last);
}

int CTorus::isLiving(int x, int y) {
    if (x < 0) {
        x = (x + length) % length;
    } else {
        x = x % length;
    }
    if (y < 0) {
        y = (y + height) % height;
    } else {
        y = y % height;
    }
    return torus[y * length + x];
}

int CTorus::numOfNeighbours(int x, int y) {
    int a[8][2] = {{-1, -1},
                   {0,  -1},
                   {1,  -1},
                   {-1, 0},
                   {1,  0},
                   {-1, 1},
                   {0,  1},
                   {1,  1}};
    int res = 0;
    for (int i = 0; i < 8; ++i) {
        if ((isLiving(x + a[i][0], y + a[i][1])) == settled) {
            res++;
        }
    }
    return res;
}

void CTorus::action(int act, int wasHere, int x, int y) {
    if (act == EXTERMINATE) {
        torus[y * length + x] = empty;
    } else if (act == SETTLE) {
        torus[y * length + x] = settled;
    } else {
        torus[y * length + x] = wasHere;
    }
}

int CTorus::countCell(int x, int y) {
    int numOfNeighb = numOfNeighbours(x, y);
    if (numOfNeighb == 3 && (torus[y * length + x] == empty)) {
        return SETTLE;
    } else if ((numOfNeighb > 3 || numOfNeighb < 2) && (torus[y * length + x] == settled)) {
        return EXTERMINATE;
    } else {
        return NOTHING;
    }
}

void CTorus::set_nothing(int x, int y) {
    torus[y * length + x] = notchanged;
}

int CTorus::getCell(int x, int y) {
    return torus[y * length + x];
}






