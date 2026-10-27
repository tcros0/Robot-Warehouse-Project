
#include "Robot.h"

// Create a robot with full battery
Robot::Robot(int robotId, Position startPosition,
             int width, int height)
    : id(robotId),
      position(startPosition),
      batteryLevel(100),
      status("Idle"),
      carriedItem(nullptr),
      currentTaskId(-1),
      chargeRate(10),
      warehouseWidth(width),
      warehouseHeight(height)
{
}

// Get robot information
int Robot::getId() const
{
    return id;
}

Position Robot::getPosition() const
{
    return position;
}

int Robot::getBatteryLevel() const
{
    return batteryLevel;
}

std::string Robot::getStatus() const
{
    return status;
}

// Get the current task ID
int Robot::getCurrentTaskId() const
{
    return currentTaskId;
}

// Move the robot up
bool Robot::moveUp()
{
    if (batteryLevel <= 0 || status == "Broken" ||
        position.getYPosition() <= 0)
        return false;

    position.setYPosition(position.getYPosition() - 1);
    drainBattery(1);
    return true;
}

// Move the robot down
bool Robot::moveDown()
{
    if (batteryLevel <= 0 || status == "Broken" ||
        position.getYPosition() >= warehouseHeight - 1)
        return false;

    position.setYPosition(position.getYPosition() + 1);
    drainBattery(1);
    return true;
}

// Move the robot left
bool Robot::moveLeft()
{
    if (batteryLevel <= 0 || status == "Broken" ||
        position.getXPosition() <= 0)
        return false;

    position.setXPosition(position.getXPosition() - 1);
    drainBattery(1);
    return true;
}

// Move the robot right
bool Robot::moveRight()
{
    if (batteryLevel <= 0 || status == "Broken" ||
        position.getXPosition() >= warehouseWidth - 1)
        return false;

    position.setXPosition(position.getXPosition() + 1);
    drainBattery(1);
    return true;
}

// Use battery when the robot moves
void Robot::drainBattery(int amount)
{
    if (amount <= 0)
        return;

    batteryLevel -= amount;

    if (batteryLevel <= 0)
    {
        batteryLevel = 0;
        status = "Out of Battery";
    }
}

// Check if the battery is low
bool Robot::isLowBattery() const
{
    return batteryLevel <= LOW_BATTERY;
}

// Charge the robot battery
void Robot::chargeBattery()
{
    if (status == "Broken")
        return;

    batteryLevel += chargeRate;

    if (batteryLevel > 100)
        batteryLevel = 100;

    if (status == "Out of Battery" && batteryLevel > 0)
        status = "Idle";
}

// Pick up an item
bool Robot::pickUpItem(std::shared_ptr<Item> item)
{
    if (!item || carriedItem ||
        batteryLevel <= 0 || status == "Broken")
        return false;

    carriedItem = item;
    status = "Busy";
    return true;
}

// Drop off an item
std::shared_ptr<Item> Robot::dropOffItem()
{
    if (!carriedItem)
        return nullptr;

    std::shared_ptr<Item> droppedItem = carriedItem;
    carriedItem.reset();

    if (batteryLevel > 0 && status != "Broken")
        status = "Idle";

    return droppedItem;
}

// Check if the robot has an item
bool Robot::isCarryingItem() const
{
    return carriedItem != nullptr;
}

// Set a new route for the robot
void Robot::setRoute(const std::vector<Position>& newRoute)
{
    route = newRoute;
}

// Set the robot's current task
void Robot::setCurrentTaskId(int taskId)
{
    currentTaskId = taskId;
}

// Move the robot one step along its route
bool Robot::moveAlongRoute()
{
    if (route.empty() || batteryLevel <= 0 ||
        status == "Broken")
        return false;

    Position next = route.front();

    int dx = next.getXPosition() - position.getXPosition();
    int dy = next.getYPosition() - position.getYPosition();

    bool moved = false;

    if (dx == 1 && dy == 0)
        moved = moveRight();
    else if (dx == -1 && dy == 0)
        moved = moveLeft();
    else if (dx == 0 && dy == 1)
        moved = moveDown();
    else if (dx == 0 && dy == -1)
        moved = moveUp();
    else if (dx == 0 && dy == 0)
        moved = true;

    if (moved)
        route.erase(route.begin());

    return moved;
}
