// Task implementation

#include "Task.h"

// Create a new task
Task::Task(int id, int itemId, Position pickup, Position delivery)
    : taskId(id),
      itemId(itemId),
      pickupLocation(pickup),
      deliveryLocation(delivery),
      status("Pending"),
      assignedRobotId(-1)
{
}

// Get task ID
int Task::getTaskId() const
{
    return taskId;
}

// Get item ID
int Task::getItemId() const
{
    return itemId;
}

// Get pickup location
Position Task::getPickupLocation() const
{
    return pickupLocation;
}

// Get delivery location
Position Task::getDeliveryLocation() const
{
    return deliveryLocation;
}

// Get task status
std::string Task::getStatus() const
{
    return status;
}

// Get assigned robot ID
int Task::getAssignedRobotId() const
{
    return assignedRobotId;
}

// Update task status
void Task::setStatus(const std::string& newStatus)
{
    status = newStatus;
}

// Assign a robot to the task
void Task::assignRobot(int robotId)
{
    assignedRobotId = robotId;
    status = "Assigned";
}
