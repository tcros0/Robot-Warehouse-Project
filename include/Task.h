#ifndef TASK_H
#define TASK_H

#pragma once

#include "Position.h"
#include <string>

// Task class
class Task
{
private:
    // Task information
    int taskId;
    int itemId;

    // Pickup and delivery locations
    Position pickupLocation;
    Position deliveryLocation;

    // Task status and assigned robot
    std::string status;
    int assignedRobotId;

public:
    // Create a task
    Task(int id, int itemId, Position pickup,
         Position delivery);

    // Get task information
    int getTaskId() const;
    int getItemId() const;

    // Get task locations
    Position getPickupLocation() const;
    Position getDeliveryLocation() const;

    // Get task status
    std::string getStatus() const;
    int getAssignedRobotId() const;

    // Update task status
    void setStatus(const std::string& newStatus);

    // Assign a robot to the task
    void assignRobot(int robotId);
};


#endif //TASK_H

