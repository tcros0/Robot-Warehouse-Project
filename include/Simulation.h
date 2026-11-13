#ifndef SIMULATION_H
#define SIMULATION_H

#include "Warehouse.h"
#include "Robot.h"
#include "TaskManager.h"
#include "PathFinding.h"
#include "RobotManager.h"

#include <vector>

#include "SimulationConfig.h"

class Simulation {
    friend class FileManager;


private:
    SimulationConfig config;
    Warehouse warehouse;
    TaskManager taskManager;
    PathFinding pathfinding;
    RobotManager robotManager;


    std::vector<Robot> robots;

    int currentTime{};
    void sendToCharge(Robot& r);
    void continueTask(Robot& r);

    void ShowPath();
    void displayPath(const std::vector<Position>& path) const;

    int taskID{};

public:

    Simulation(const SimulationConfig& config);
    ~Simulation();

    void run(); //Starts simulation
    void advanceTime();

    void createTask();
    void assignTask();
    void inspectRobot();
    void showTaskStatus();
        
    void createWarehouseLayout();
    void editWarehouse();
        
    void setTaskID(int id);



    void buildFromConfig();//uses config to reset map

    template <typename T>
    T getInput(const std::string& prompt);
    template <typename T>
    T getInput(const std::string& prompt, T min, T max);



};

#endif //SIMULATION_H
