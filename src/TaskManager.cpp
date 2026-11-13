
#include "TaskManager.h"

// Create a task manager
TaskManager::TaskManager()
    : nextTaskId(1)
{
}

// Destroy the task manager
TaskManager::~TaskManager()
{
}

// Create a new task
void TaskManager::createTask(int itemId, Position pickup,
                             Position delivery)
{
    tasks.emplace_back(nextTaskId, itemId, pickup, delivery);
    nextTaskId++;
}

// Check if a robot is available
bool TaskManager::isRobotAvailable(const Robot& robot) const
{
    return robot.getStatus() == "Idle" &&
           robot.getCurrentTaskId() == -1 &&
           robot.getBatteryLevel() > 20;
}

// Check if a pickup location is available
bool TaskManager::isPickupAvailable(Position pickup) const
{
    // Check active tasks at this pickup location
    for (const Task& task : tasks)
    {
        Position location = task.getPickupLocation();

        if (location.getXPosition() == pickup.getXPosition() &&
            location.getYPosition() == pickup.getYPosition() &&
            task.getStatus() != "Completed")
        {
            return false;
        }
    }

    return true;
}

// Assign a task to a robot
bool TaskManager::assignTask(int taskId, Robot& robot)
{
    Task* task = getTask(taskId);

    if (task == nullptr ||
        task->getStatus() != "Pending" ||
        !isRobotAvailable(robot))
    {
        return false;
    }

    task->assignRobot(robot.getId());
    robot.setCurrentTaskId(taskId);

    return true;
}

// Get all tasks
const std::vector<Task>& TaskManager::getTasks() const
{
    return tasks;
}

// Find a task by ID
Task* TaskManager::getTask(int taskId)
{
    for (Task& task : tasks)
    {
        if (task.getTaskId() == taskId)
        {
            return &task;
        }
    }

    return nullptr;
}

// Complete a task
bool TaskManager::markTaskCompleted(int taskId)
{
    Task* task = getTask(taskId);

    if (task == nullptr ||
        task->getStatus() == "Completed")
    {
        return false;
    }

    task->setStatus("Completed");
    return true;
}

// Reassign a task to another robot
bool TaskManager::reassignTask(int taskId, Robot& robot)
{
    Task* task = getTask(taskId);

    if (task == nullptr ||
        task->getStatus() == "Completed" ||
        !isRobotAvailable(robot))
    {
        return false;
    }

    task->assignRobot(robot.getId());
    robot.setCurrentTaskId(taskId);

    return true;
}


// Empties list
void TaskManager::resetList() {
    tasks.clear();
    nextTaskId = 1;
}
