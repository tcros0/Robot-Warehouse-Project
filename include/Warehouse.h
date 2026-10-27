#pragma once
#include <vector>
#include <Item.h>
#include <Position.h>


struct pickupPoint {
    int ID;
    Position positionPU;
    std::vector<Item> items;
};

struct deliveryPoint {
    int ID;
    Position positionD;
    std::vector<Item> items;
};

class Warehouse {
private:
    int length{};
    int width{};
    std::vector<std::vector<char>> warehouseMap;
    std::vector<pickupPoint> listPickupPoints;
    std::vector<deliveryPoint> listDeliveryPoints;

public:
    Warehouse();
    Warehouse(int length, int width);

    void createMap(int length, int width);
    bool isInBounds(int row, int col) const;

    void setPickupPoint(int row, int col, int ID);
    void setDeliveryPoint(int row, int col, int ID);
    void setChargingPoint(int row, int col, int ID);

    Position getPickupLocation(int id) const;
    Position getDeliveryLocation(int id) const;
    Position getChargingLocation(int id) const;

    bool setCell(int row, int col, char symbol);
    char getCell(int row, int col) const;
    void display() const;

    ~Warehouse();
};
