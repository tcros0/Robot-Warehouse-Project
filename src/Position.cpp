#include "Position.h"

Position::Position() {
    xPos = 0;
    yPos = 0;
};

Position::Position(int x, int y) {
    xPos = x;
    yPos = y;
};

int Position::getXPosition() const {
    return xPos;
}

int Position::getYPosition() const{
    return yPos;
}

void Position::setXPosition(int x) {
    xPos = x;
}

void Position::setYPosition(int y) {
    yPos = y;
}

Position::~Position(){};
