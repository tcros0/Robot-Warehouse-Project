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
    std::vector<int> warehouse;

public:
    Warehouse();
    Warehouse(int length, int width);
    pickupPoint p1;
    deliveryPoint d1;

    ~Warehouse();
};
