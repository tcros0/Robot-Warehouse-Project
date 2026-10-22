
#pragma once

#include "Position.h"
#include "Item.h"
#include <string>
#include <optional>

class Robot {
private:
    int id;
    Position position;
    int batteryLevel;
    std::string status;
    std::optional<Item> carriedItem;

public:
    Robot(int robotId, Position startPosition);

    int getId() const;
    Position getPosition() const;
    int getBatteryLevel() const;
    std::string getStatus() const;

    bool moveUp();
    bool moveDown();
    bool moveLeft();
    bool moveRight();

    void drainBattery(int amount);
    bool pickUpItem(const Item& item);
    bool dropOffItem();
    bool isCarryingItem() const;
};

