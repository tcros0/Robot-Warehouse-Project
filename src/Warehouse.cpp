#include "Warehouse.h"
#include<iostream>

Warehouse::Warehouse() {}
Warehouse::Warehouse(int length, int width) {
    this->length = length;
    this->width = width;

    createMap(length, width);
}

bool Warehouse::isInBounds(int x, int y) const {
    if (x>length || y>width) {
        return false;
    }
    else {
        return true;
    }
}

//Pickup points are denoted with a P
void Warehouse::setPickupPoint(int row, int col, int ID) {


    if (warehouseMap[row][col] != '.') {
            std::cout<<"Obstacle already exists at that spot" << std::endl;
            return;
    }



    pickupPoint newPoint;
    newPoint.positionPU.setXPosition(row);
    newPoint.positionPU.setYPosition(col);
    newPoint.ID = ID;

    listPickupPoints.push_back(newPoint);
    warehouseMap[row][col] = 'P';

}

//Delivery points denoted with a D
void Warehouse::setDeliveryPoint(int row, int col, int ID) {

}
Warehouse::~Warehouse() {}