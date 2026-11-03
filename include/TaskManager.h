

#pragma once

#include "Task.h"
#include "Robot.h"
#include "Position.h"
#include <vector>

// TaskManager class
class TaskManager
{
private:
    // Store all tasks
    std::vector<Task> tasks;

    // Generate task IDs
    int nextTaskId;

public:
    // Create a task manager
    TaskManager();

    // Create a new task
    void createTask(int itemId, Position pickup,
                    Position delivery);

    // Check if a robot is available
    bool isRobotAvailable(const Robot& robot) const;

    // Check if a pickup location is available
    bool isPickupAvailable(Position pickup) const;

    // Assign a task to a robot
    bool assignTask(int taskId, Robot& robot);

    // Get all tasks
    const std::vector<Task>& getTasks() const;

    // Find a task by ID
    Task* getTask(int taskId);

    // Complete a task
    bool markTaskCompleted(int taskId);

    // Reassign a task
    bool reassignTask(int taskId, Robot& robot);
};

