

#pragma once

#include "Position.h"
#include "Item.h"
#include <string>
#include <vector>
#include <memory>

class Robot {
private:
    static const int LOW_BATTERY = 20;

    int id;
    Position position;
    int batteryLevel;
    std::string status;
    std::shared_ptr<Item> carriedItem;

    std::vector<Position> route;
    int currentTaskId;
    int chargeRate;

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
    bool moveAlongRoute();

    void drainBattery(int amount);
    bool isLowBattery() const;
    void chargeBattery(int amount);

    bool pickUpItem(std::shared_ptr<Item> item);
    bool dropOffItem();
    bool isCarryingItem() const;

    void setRoute(const std::vector<Position>& newRoute);
    void setCurrentTaskId(int taskId);
    int getCurrentTaskId() const;
};
