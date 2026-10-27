
#pragma once

#include "Position.h"
#include "Item.h"
#include <string>
#include <vector>
#include <memory>

// Robot class
class Robot
{
private:
    // Low battery warning at 20%
    static const int LOW_BATTERY = 20;

    // Robot information
    int id;
    Position position;
    int batteryLevel;
    std::string status;

    // Item carried by the robot
    std::shared_ptr<Item> carriedItem;

    // Robot route and task
    std::vector<Position> route;
    int currentTaskId;
    int chargeRate;

    // Warehouse size
    int warehouseWidth;
    int warehouseHeight;

public:
    // Create a robot
    Robot(int robotId, Position startPosition,
          int width, int height);

    // Get robot information
    int getId() const;
    Position getPosition() const;
    int getBatteryLevel() const;
    std::string getStatus() const;

    // Move the robot
    bool moveUp();
    bool moveDown();
    bool moveLeft();
    bool moveRight();

    // Follow the robot's route
    bool moveAlongRoute();

    // Manage robot battery
    void drainBattery(int amount);
    bool isLowBattery() const;
    void chargeBattery();

    // Pick up and drop off items
    bool pickUpItem(std::shared_ptr<Item> item);
    std::shared_ptr<Item> dropOffItem();
    bool isCarryingItem() const;

    // Set a route for the robot
    void setRoute(const std::vector<Position>& newRoute);

    // Manage the robot's task
    void setCurrentTaskId(int taskId);
    int getCurrentTaskId() const;
};
