#ifndef SIMULATION_H
#define SIMULATION_H

#include "Warehouse.h"
#include "Robot.h"
#include "TaskManager.h"
#include "FileManager.h"
#include "PathFinding.h"

#include <vector>

#include "SimulationConfig.h"

class Simulation {

private:
    SimulationConfig config;
    Warehouse warehouse;
    TaskManager taskManager;
    PathFinding pathfinding;

    std::vector<Robot> robots;

    int currentTime{};
    void sendToCharge(Robot& r);
    void continueTask(Robot& r);

public:

    Simulation(const SimulationConfig& config);
    ~Simulation();

    void run(); //Starts simulation
    void advanceTime();
    void resetSimulation();

    void createTask();
    void assignTask();
    void inspectRobot();
    void showTaskStatus();

    template <typename T>
    T getInput(const std::string& prompt);
    template <typename T>
    T getInput(const std::string& prompt, T min, T max);



};

#endif //SIMULATION_H