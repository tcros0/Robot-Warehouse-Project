#pragma once
#include <vector>
#include "Item.h"
#include "Position.h"


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

struct chargingPoint {
    int ID;
    static int constexpr chargeRate = 10;
    Position positionC;

};

class Warehouse {
private:
    int length{};
    int width{};
    std::vector<std::vector<char>> warehouseMap;
    std::vector<pickupPoint> listPickupPoints;
    std::vector<deliveryPoint> listDeliveryPoints;
    std::vector<chargingPoint> listChargingPoints;

public:
    Warehouse();
    Warehouse(int length, int width);

    void createMap(); // default uses info from txt file
    void createMap(int length, int width); //used for manual creation of map
    bool isInBounds(int row, int col) const;

    void setPickupPoint(int row, int col, int ID);
    void setDeliveryPoint(int row, int col, int ID);
    void setChargingPoint(int row, int col, int ID);

    void displayPickupLocations() const;
    void displayDeliveryLocations() const;
    void displayChargingLocation();

    Position getPickupLocation(int id) const;
    Position getDeliveryLocation(int id) const;
    Position getChargingLocation(int id) const;

    //returns size of warehouse
    int getLength() const;
    int getWidth() const;

    bool setCell(int row, int col, char symbol);
    char getCell(int row, int col) const;
    void display() const;

    ~Warehouse();
};
