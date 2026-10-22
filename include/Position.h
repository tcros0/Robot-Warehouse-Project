#pragma once
#ifndef POSITION_H
#define POSITION_H

class Position {
private:
    int xPos;
    int yPos;
public:
    Position();
    Position(int x, int y);

    int getXPosition() const;
    int getYPosition() const;
    void setXPosition(int x);
    void setYPosition(int y);

    ~Position();

};

#endif
