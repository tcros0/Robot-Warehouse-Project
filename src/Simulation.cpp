#include "../include/Simulation.h"
#include "SimulationConfig.h"
#include "PathFinding.h"


#include <iostream>
#include <limits>
#include <string>

Simulation::Simulation(const SimulationConfig& config) {

    this->config = config;
    buildFromConfig();


} //initialize robot: ID, position. Warehouse: length and width


void Simulation::buildFromConfig() {

    warehouse = Warehouse();
    robots.clear();
    currentTime = 0;

    //Creates x by y map size
    warehouse.createMap(config.length, config.width);

    //Places deliveryPoints from txt file
    for (int i = 0; i < config.deliveryPoints.size(); i++) {
        warehouse.setDeliveryPoint(config.deliveryPoints[i].positionD.getXPosition(), config.deliveryPoints[i].positionD.getYPosition(), config.deliveryPoints[i].ID);
    }

    //Populates items list into deliveryPoints
    for (int i = 0; i < config.deliveryPoints.size(); i++) {
        for (int o = 0; o<config.deliveryPoints[i].items.size(); o++) {
            warehouse.addListDeliveryItems(config.deliveryPoints[i].ID, config.deliveryPoints[i].items[o]);
        }
    }

    //Places pickupPoints from txt file
    for (int i = 0; i < config.pickupPoints.size(); i++) {
        warehouse.setPickupPoint(config.pickupPoints[i].positionPU.getXPosition(), config.pickupPoints[i].positionPU.getYPosition(), config.pickupPoints[i].ID);
    }

    //Populates items list into pickUpPoints
    for (int i = 0; i < config.pickupPoints.size(); i++) {
        for (int o = 0; o<config.pickupPoints[i].items.size(); o++) {
            warehouse.addListPickUpItems(config.pickupPoints[i].ID, config.pickupPoints[i].items[o]);
        }
    }

    //Places chargingPoints from txt file
    for (int i = 0; i < config.chargingPoints.size(); i++) {
        warehouse.setChargingPoint(config.chargingPoints[i].positionC.getXPosition(), config.chargingPoints[i].positionC.getYPosition(), config.chargingPoints[i].ID, config.chargingPoints[i].free);
    }

    //Places walls in warehouse
    for (int i = 0; i < config.obstacles.size();i++) {
        warehouse.setCell(config.obstacles[i].getXPosition(), config.obstacles[i].getYPosition(), '#');
    }
    //Populates robot vector with robot objects IF they are in bounds and not on a wall
    for (const auto& rc : config.robots) {
        if (warehouse.isInBounds(rc.position.getXPosition(), rc.position.getYPosition())&&(warehouse.getCell(rc.position.getXPosition(), rc.position.getYPosition()) != '#')) {
            robots.emplace_back(rc.ID, rc.position, rc.battery);
        }
        else {
            std::cout << "Robot " << rc.ID << " skipped: start position (" << rc.position.getXPosition()  << ", " << rc.position.getYPosition() << ") is out of bounds or a wall.\n";
        }
    }

}

void Simulation::ShowPath() {
    //Show where the points are so the user knows what coordinates to enter
    warehouse.displayPickupLocations();
    warehouse.displayDeliveryLocations();
    warehouse.displayChargingLocation();

    bool done = false;


    //x = row, y = column
    int startX = getInput<int>("Start row (0-" + std::to_string(config.length - 1) + "): ", 0, config.length - 1);
    int startY = getInput<int>("Start column (0-" + std::to_string(config.width - 1) + "): ", 0, config.width - 1);
    int goalX = getInput<int>("Goal row (0-" + std::to_string(config.length - 1) + "): ", 0, config.length - 1);
    int goalY = getInput<int>("Goal column (0-" + std::to_string(config.width - 1) + "): ", 0, config.width - 1);

    while (!done) {
        if (warehouse.getCell(startX, startY) == '#') {
            std::cout << "Start is on a wall.\n";
            startX = getInput<int>("Start row (0-" + std::to_string(config.length - 1) + "): ", 0, config.length - 1);
            startY = getInput<int>("Start column (0-" + std::to_string(config.width - 1) + "): ", 0, config.width - 1);
        }
        else {
            done = true;
        }
    }

    done = false;
    while (!done) {
        if (startX == goalX && startY == goalY) {
            std::cout << "Start and goal are the same cell.\n";
            goalX = getInput<int>("Goal row (0-" + std::to_string(config.length - 1) + "): ", 0, config.length - 1);
            goalY = getInput<int>("Goal column (0-" + std::to_string(config.width - 1) + "): ", 0, config.width - 1);

        }
        else {
            done = true;
        }
    }

    Position start(startX, startY);
    Position goal(goalX, goalY);

    std::vector<Position> path = pathfinding.findPath(warehouse, start, goal);
    displayPath(path);

}

void Simulation::displayPath(const std::vector<Position>& path) const {

    if (path.empty()) {
        std::cout << "No path found (goal is unreachable or a wall).\n";
        return;
    }

    std::cout << "Path:" << std::endl;
    for (int x = 0; x < config.length; x++) {
        for (int y = 0; y < config.width; y++) {
            bool onPath = false;
            for (const Position& p : path) {
                if (p.getXPosition() == x && p.getYPosition() == y) {
                    onPath = true;
                    break;
                }
            }

            //Only draw '*' on empty floor so P, D and C stay visible
            if (onPath && warehouse.getCell(x, y) == '.') {
                std::cout << '*';
            }
            else {
                std::cout << warehouse.getCell(x, y);
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Path length: " << path.size() << " steps" << std::endl;
}

void Simulation::run() {

    bool done = false;
    while (!done) {
        int option{};

        std::cout<<"Warehouse Manager:"
        "\n1.See warehouse map"
        "\n2.Check robot status"
        "\n3.Create Task"
        "\n4.View Task list"
        "\n5.Create new Warehouse Layout"
        "\n6.Edit Warehouse"
        "\n7.Reset Warehouse"
        "\n8.Advance time"
        "\n9.Exit"
        "\n10.Display Path"
        "\n11.Assign Task" << std::endl;

        option = getInput<int> ("Enter Option: ", 1, 11);


        switch(option){
            case 1:
                warehouse.display();
                break;
            case 2:
                inspectRobot();
                break;
            case 3:
                createTask();
                break;
            case 4:
                showTaskStatus();
                break;
            case 5:
                std::cout<<"Nothing was implemented here yet!" << std::endl;
                break;
            case 6:
                std::cout<<"Nothing was implemented here yet!" << std::endl;
                break;
            case 7:
                buildFromConfig();
                std::cout<<"Warehouse was reset to initial settings";
                break;
            case 8:
                std::cout<<"Nothing was implemented here yet!" << std::endl;
                break;
            case 9:
                done = true;
                break;
            case 10:
                ShowPath();
                break;
            case 11:
                assignTask();
                break; 
        }
    }

}

void Simulation::advanceTime() {//updates everything
    currentTime++;
    //need robotManager

    //check robot battery levels

    std::vector<Robot> robotList= robotManager.getRobots();
    for (int i = 0; i<robotList.size();i++) {
        if (robotList[i].isLowBattery()) {
            robotList[i].setStatus("lowBattery");
        }

        if (robotList[i].isLowBattery()&& robotList[i].getStatus() != "lowBattery") {
            robotList[i].setStatus("toCharge");
            sendToCharge(robotList[i]);
        }
    }
}

void Simulation::setTaskID(int id) {
    taskID = id;
}

//grabs an input and validates it using getInput<dataType>()
template <typename T>
T Simulation::getInput(const std::string& prompt) {
    T value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            // throw away anything left on the line (e.g. "5abc" -> "abc")
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();   // reset the failed state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, try again.\n";
    }
}

// same thing, but the value must be between min and max (inclusive)
template <typename T>
T Simulation::getInput(const std::string& prompt, T min, T max) {
    while (true) {
        T value = getInput<T>(prompt);
        if (value >= min && value <= max) return value;
        std::cout << "Please enter a value from " << min << " to " << max << ".\n";
    }
}

void Simulation::sendToCharge(Robot& r) {
    Position rPosition = r.getPosition();
    Position chargePoint{};
    std::vector<Position> path;
    PathFinding PathFinder;

    setTaskID(r.getCurrentTaskId());

    int index{};
    bool freeSpot = false;

    for (int i = 0; i<warehouse.listChargingPoints.size(); i++) {
        if (warehouse.listChargingPoints[i].free == true){
            std::vector<Position> path1;

            Position chargePoint1{};

            chargePoint1.setXPosition(warehouse.listChargingPoints[i].positionC.getXPosition());
            chargePoint1.setYPosition(warehouse.listChargingPoints[i].positionC.getYPosition());


            path1 = PathFinder.findPath(warehouse, rPosition, chargePoint1);

            if (!path1.empty() && (path.empty() || path1.size() < path.size())) {
                path = path1;
                index = i;
                freeSpot = true;
            }

        }
    }
    if (!freeSpot) {
        std::cout <<"No available charging stations at the moment" << std::endl;
        return;
    }

    std::cout<<"Sending Robot to " << warehouse.listChargingPoints[index].ID << " charger..." << std::endl;

    r.setRoute(path);
}

void Simulation::continueTask(Robot &r) {
    PathFinding PathFinder;

    int taskID = r.getCurrentTaskId();

    if (taskID == -1 || taskManager.getTask(taskID) == nullptr) {
        return;
    }

    Position taskPickUp = taskManager.getTask(taskID)->getPickupLocation();
    Position taskDelivery = taskManager.getTask(taskID)->getDeliveryLocation();
    std::vector<Position> path;
    Position rPosition = r.getPosition();

    if (!r.isCarryingItem()) {
        path = PathFinder.findPath(warehouse, rPosition,taskPickUp);
    }
    else {
        path = PathFinder.findPath(warehouse, rPosition, taskDelivery);
    }

    if (path.empty()) {
        std::cout << "Path cannot be completed" << std::endl;
        return;
    }
    r.setRoute(path);

}

Simulation::~Simulation(){}

 // Display all robots
void Simulation::inspectRobot()
{
    if (robots.empty())
    {
        std::cout << "No robots available.\n";
        return;
    }

    for (const Robot& robot : robots)
    {
        Position pos = robot.getPosition();

        std::cout << "\nRobot ID: " << robot.getId()
                  << "\nPosition: (" << pos.getXPosition()
                  << ", " << pos.getYPosition() << ")"
                  << "\nBattery: " << robot.getBatteryLevel() << "%"
                  << "\nStatus: " << robot.getStatus()
                  << "\nCurrent Task: " << robot.getCurrentTaskId()
                  << "\n";
    }
}

void Simulation::showTaskStatus()
{
    const std::vector<Task>& tasks = taskManager.getTasks();

    if (tasks.empty())
    {
        std::cout << "No tasks available.\n";
        return;
    }

    for (const Task& task : tasks)
    {
        Position pickup = task.getPickupLocation();
        Position delivery = task.getDeliveryLocation();

        std::cout << "\nTask ID: " << task.getTaskId()
                  << "\nItem ID: " << task.getItemId()
                  << "\nPickup: (" << pickup.getXPosition()
                  << ", " << pickup.getYPosition() << ")"
                  << "\nDelivery: (" << delivery.getXPosition()
                  << ", " << delivery.getYPosition() << ")"
                  << "\nStatus: " << task.getStatus()
                  << "\nAssigned Robot: " << task.getAssignedRobotId()
                  << "\n";
    }
}

void Simulation::createTask()
{
    int itemId = getInput<int>("Enter Item ID: ", 1, 1000000);

    int pickupX = getInput<int>(
        "Pickup row: ", 0, config.length - 1);

    int pickupY = getInput<int>(
        "Pickup column: ", 0, config.width - 1);

    int deliveryX = getInput<int>(
        "Delivery row: ", 0, config.length - 1);

    int deliveryY = getInput<int>(
        "Delivery column: ", 0, config.width - 1);

    Position pickup(pickupX, pickupY);
    Position delivery(deliveryX, deliveryY);

    if (warehouse.getCell(pickupX, pickupY) == '#' ||
        warehouse.getCell(deliveryX, deliveryY) == '#')
    {
        std::cout << "Location cannot be a wall.\n";
        return;
    }

    if (!taskManager.isPickupAvailable(pickup))
    {
        std::cout << "Pickup location is already in use.\n";
        return;
    }

    taskManager.createTask(itemId, pickup, delivery);

    std::cout << "Task created successfully!\n";
}

void Simulation::assignTask()
{
    int taskId = getInput<int>("Enter Task ID: ");
    int robotId = getInput<int>("Enter Robot ID: ");

    for (Robot& robot : robots)
    {
        if (robot.getId() == robotId)
        {
            if (taskManager.assignTask(taskId, robot))
            {
                std::cout << "Task assigned successfully!\n";
            }
            else
            {
                std::cout << "Unable to assign task.\n";
            }
            return;
        }
    }

    std::cout << "Robot not found.\n";
}





