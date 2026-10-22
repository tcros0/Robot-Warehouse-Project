
#include "Robot.h"

// Constructor
Robot::Robot(int robotId, Position startPosition)
    : id(robotId),
      position(startPosition),
      batteryLevel(100),
      status("Idle"),
      carriedItem(std::nullopt)
{
}

// Get robot ID
int Robot::getId() const
{
    return id;
}

// Get robot position
Position Robot::getPosition() const
{
    return position;
}

// Get battery level
int Robot::getBatteryLevel() const
{
    return batteryLevel;
}

// Get robot status
std::string Robot::getStatus() const
{
    return status;
}

// Move robot up
bool Robot::moveUp()
{
    if (batteryLevel <= 0 || status == "Broken" ||
        position.getYPosition() <= 0)
    {
        return false;
    }

    position.setYPosition(position.getYPosition() - 1);
    drainBattery(1);
    return true;
}

// Move robot down
bool Robot::moveDown()
{
    if (batteryLevel <= 0 || status == "Broken" ||
        position.getYPosition() >= 14)
    {
        return false;
    }

    position.setYPosition(position.getYPosition() + 1);
    drainBattery(1);
    return true;
}

// Move robot left
bool Robot::moveLeft()
{
    if (batteryLevel <= 0 || status == "Broken" ||
        position.getXPosition() <= 0)
    {
        return false;
    }

    position.setXPosition(position.getXPosition() - 1);
    drainBattery(1);
    return true;
}

// Move robot right
bool Robot::moveRight()
{
    if (batteryLevel <= 0 || status == "Broken" ||
        position.getXPosition() >= 24)
    {
        return false;
    }

    position.setXPosition(position.getXPosition() + 1);
    drainBattery(1);
    return true;
}

// Reduce battery level
void Robot::drainBattery(int amount)
{
    if (amount <= 0)
    {
        return;
    }

    batteryLevel -= amount;

    if (batteryLevel <= 0)
    {
        batteryLevel = 0;
        status = "Out of Battery";
    }
}

// Pick up an item
bool Robot::pickUpItem(const Item& item)
{
    if (carriedItem.has_value() ||
        batteryLevel <= 0 ||
        status == "Broken")
    {
        return false;
    }

    carriedItem = item;
    status = "Busy";
    return true;
}

// Drop off an item
bool Robot::dropOffItem()
{
    if (!carriedItem.has_value())
    {
        return false;
    }

    carriedItem.reset();

    if (batteryLevel > 0 && status != "Broken")
    {
        status = "Idle";
    }

    return true;
}

// Check if robot is carrying an item
bool Robot::isCarryingItem() const
{
    return carriedItem.has_value();
}




