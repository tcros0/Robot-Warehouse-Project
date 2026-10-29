#include "Warehouse.h"
#include<iostream>

Warehouse::Warehouse() {}
Warehouse::Warehouse(int length, int width) {
    this->length = length;
    this->width = width;

    createMap(length, width);
}

bool Warehouse::isInBounds(int x, int y) const {
    if (x>=length || y>=width||x<0 || y<0) {
        std::cout<<"Coordinates are not inside the map" << std::endl;
        return false;
    }
    return true;

}

void Warehouse::createMap() {

    warehouseMap.assign(length, std::vector<char>(width, '.'));

    //Draws the Pickup Point
    for (int i = 0; i<listPickupPoints.size(); i++) {
        int x = listPickupPoints[i].positionPU.getXPosition();
        int y = listPickupPoints[i].positionPU.getYPosition();

        warehouseMap[x][y] = 'P';
    }

    //Draws the Delivery Points
    for (int i = 0; i<listDeliveryPoints.size(); i++) {
        int x = listDeliveryPoints[i].positionD.getXPosition();
        int y = listDeliveryPoints[i].positionD.getYPosition();

        warehouseMap[x][y] = 'D';
    }

    //Draws Charging points
    for (int i = 0; i<listChargingPoints.size(); i++) {
        int x = listChargingPoints[i].positionC.getXPosition();
        int y = listChargingPoints[i].positionC.getYPosition();

        warehouseMap[x][y] = 'C';
    }

    std::cout << "Warehouse created" << std::endl;
}

void Warehouse::createMap(int x, int y) {
    warehouseMap.assign(x, std::vector<char>(y, '.'));

}

//Pickup points are denoted with a P
void Warehouse::setPickupPoint(int row, int col, int ID) {

    if (!isInBounds(row, col)) {
        return;
    }

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

    if (!isInBounds(row, col)) {
        return;
    }

    if (warehouseMap[row][col] != '.') {
        std::cout<<"Obstacle already exists at that spot" << std::endl;
        return;
    }

    deliveryPoint newPoint;
    newPoint.positionD.setXPosition(row);
    newPoint.positionD.setYPosition(col);
    newPoint.ID = ID;

    listDeliveryPoints.push_back(newPoint);
    warehouseMap[row][col] = 'D';
}

void Warehouse::setChargingPoint(int row, int col, int ID) {

    if (!isInBounds(row, col)) {
        return;
    }

    if (warehouseMap[row][col] != '.') {
        std::cout<<"Obstacle already exists at that spot" << std::endl;
        return;
    }

    chargingPoint newPoint;
    newPoint.positionC.setXPosition(row);
    newPoint.positionC.setYPosition(col);
    newPoint.ID = ID;

    listChargingPoints.push_back(newPoint);
    warehouseMap[row][col] = 'C';
}

void Warehouse::displayPickupLocations() const{
    std::cout<<"Pickup Locations: " << std::endl;
    for (int i = 0; i<listPickupPoints.size(); i++) {
        std::cout<<"Pickup point ID: " << listPickupPoints[i].ID << std::endl;
        std::cout<< "x: " << listPickupPoints[i].positionPU.getXPosition() << " y: " <<listPickupPoints[i].positionPU.getYPosition() << std::endl;;
    }
}

void Warehouse::displayDeliveryLocations() const{
    std::cout<<"Delivery Locations: " << std::endl;
    for (int i = 0; i<listDeliveryPoints.size(); i++) {
        std::cout<<"Delivery point ID: " << listDeliveryPoints[i].ID << std::endl;
        std::cout<<"x: " << listDeliveryPoints[i].positionD.getXPosition() << " y: " << listDeliveryPoints[i].positionD.getYPosition() << std::endl;
    }
}

void Warehouse::displayChargingLocation() {
    std::cout<<"Charging Locations: " << std::endl;
    for (int i = 0; i<listChargingPoints.size(); i++) {
        std::cout<<"Charging point ID: " << listChargingPoints[i].ID << std::endl;
        std::cout<<"x: " << listChargingPoints[i].positionC.getXPosition() << " y: " << listChargingPoints[i].positionC.getYPosition() << std::endl;
    }
}

Position Warehouse::getPickupLocation(int ID) const {
    return listPickupPoints[ID].positionPU;
}

Position Warehouse::getDeliveryLocation(int ID) const {
    return listDeliveryPoints[ID].positionD;
}

Position Warehouse::getChargingLocation(int ID) const {
    return listChargingPoints[ID].positionC;
}

bool Warehouse::setCell(int x, int y, char symbol) {
    if (!isInBounds(x, y)) {
        return false;
    }
    warehouseMap[x][y] = symbol;
    return true;
}

char Warehouse::getCell(int x, int y) const {
    if (!isInBounds(x, y)) {
        return ' ';
    }
    return warehouseMap[x][y];
}

void Warehouse::display() const {
    std::cout<<"Warehouse Map" << std::endl;
    for (int i = 0; i<warehouseMap.size(); i++) {
        for (int o = 0; o<warehouseMap[i].size(); o++) {
            std::cout << warehouseMap[i][o];
        }
        std::cout<<std::endl;
    }
}

Warehouse::~Warehouse() {};