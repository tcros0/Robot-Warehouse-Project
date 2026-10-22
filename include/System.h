#ifndef SIMULATION_H
#define SIMULATION_H

#include "Warehouse.h"
#include "RobotManager.h"
#include "TaskManager.h"
#include "FileManager.h"

class Simulation
{
private:
    Warehouse warehouse;        // declared here = owned by Simulation
    RobotManager robots;
    TaskManager tasks;
    FileManager fileManager;
    int currentTime;

public:
    Simulation();
    void run();
    void advanceTime();
};

#endif