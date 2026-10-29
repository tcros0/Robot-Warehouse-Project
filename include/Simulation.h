#pragma once
#include "Warehouse.h"
#include "Robot.h"
#include "TaskManager.h"
#include "FileManager.h"
#include "PathFinding.h"

#ifndef SIMULATION_H
#define SIMULATION_H

class Simulation {

private:
    Warehouse warehouse;
    FileManager filemanager;
    Robot robot;
    TaskManager taskManager;
    PathFinding pathfinding;

};

#endif //SIMULATION_H