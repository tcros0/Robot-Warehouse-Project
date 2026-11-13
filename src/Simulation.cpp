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
           robots.emplace_back(rc.ID, rc.position,
           config.width, config.length, rc.battery);
        }
        else {
            std::cout << "Robot " << rc.ID << " skipped: start position (" << rc.position.getXPosition()  << ", " << rc.position.getYPosition() << ") is out of bounds or a wall.\n";
        }
    }

    //resets task list
    taskManager.resetList();

}

void Simulation::ShowPath() {
    //Show where the points are so the user knows what coordinates to enter
    warehouse.displayPickupLocations();
    warehouse.displayDeliveryLocations();
    warehouse.displayChargingLocation();

    bool done = false;


    //x = row, y = column
    int startX = getInput<int>("Start row (0-" + std::to_string(warehouse.getLength() - 1) + "): ", 0, warehouse.getLength() - 1);
    int startY = getInput<int>("Start column (0-" + std::to_string(warehouse.getWidth() - 1) + "): ", 0, warehouse.getWidth() - 1);
    int goalX = getInput<int>("Goal row (0-" + std::to_string(warehouse.getLength() - 1) + "): ", 0, warehouse.getLength() - 1);
    int goalY = getInput<int>("Goal column (0-" + std::to_string(warehouse.getWidth() - 1) + "): ", 0, warehouse.getWidth() - 1);

    while (!done) {
        if (warehouse.getCell(startX, startY) == '#') {
            std::cout << "Start is on a wall.\n";
            startX = getInput<int>("Start row (0-" + std::to_string(warehouse.getLength() - 1) + "): ", 0, warehouse.getLength() - 1);
            startY = getInput<int>("Start column (0-" + std::to_string(warehouse.getWidth() - 1) + "): ", 0, warehouse.getWidth() - 1);
        }
        else {
            done = true;
        }
    }

    done = false;
    while (!done) {
        if (startX == goalX && startY == goalY) {
            std::cout << "Start and goal are the same cell.\n";
            goalX = getInput<int>("Goal row (0-" + std::to_string(warehouse.getLength() - 1) + "): ", 0, warehouse.getLength() - 1);
            goalY = getInput<int>("Goal column (0-" + std::to_string(warehouse.getWidth() - 1) + "): ", 0, warehouse.getWidth() - 1);

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
    for (int x = 0; x < warehouse.getLength(); x++) {
        for (int y = 0; y < warehouse.getWidth(); y++) {
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
                createWarehouseLayout();
                break;
            case 6:
                editWarehouse();
                break;     
            case 7:
                buildFromConfig();
                std::cout<<"Warehouse was reset to initial settings" << std::endl;
                break;
            case 8:
                advanceTime();
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



void Simulation::advanceTime()
{
    currentTime++;

    std::cout << "\nTime: " << currentTime << std::endl;

    for (Robot& robot : robots)
    {
        if (robot.getStatus() == "Broken" ||
            robot.getBatteryLevel() == 0)
            continue;

        // Send robot to charge when battery is low
        if (robot.isLowBattery() &&
            robot.getStatus() != "To Charge" && warehouse.chargingStationOpen())
        {
            // Only mark it as charging if a reachable charger was actually found
            if (sendToCharge(robot)) {
                robot.setStatus("To Charge");
            }
            else {
                std::cout << "Could not send Robot " << robot.getId() << " to charge" << std::endl;
            }
        }

        // Move robot along its route
        if (robot.moveAlongRoute())
        {
            std::cout << "Robot " << robot.getId()
                      << " moved.\n";
        }

        Position pos = robot.getPosition();

        // Charge robot at charging station
        if (robot.getStatus() == "To Charge")
        {
            for (auto& station :
                 warehouse.listChargingPoints)
            {
                Position charger = station.positionC;

                if (pos.getXPosition() ==
                        charger.getXPosition() &&
                    pos.getYPosition() ==
                        charger.getYPosition())
                {
                    robot.chargeBattery();

                    std::cout << "Robot "
                              << robot.getId()
                              << " charging: "
                              << robot.getBatteryLevel()
                              << "%\n";

                    if (robot.getBatteryLevel() == 100)
                    {
                        robot.setStatus("Idle");
                        station.free = true;   // release the charger reserved in sendToCharge
                        continueTask(robot);
                    }
                    break;
                }
            }
            continue;
        }

        // Check the robot's assigned task
        int taskId = robot.getCurrentTaskId();

        if (taskId == -1)
            continue;

        Task* task = taskManager.getTask(taskId);

        if (task == nullptr ||
            task->getStatus() == "Completed")
            continue;

        Position pickup = task->getPickupLocation();
        Position delivery = task->getDeliveryLocation();

        // Pick up the item at pickup location
        if (!robot.isCarryingItem() &&
            pos.getXPosition() == pickup.getXPosition() &&
            pos.getYPosition() == pickup.getYPosition())
        {
            // Take the item out of the pickup point's list
            std::shared_ptr<Item> item =
                warehouse.takeItemFromPickup(pickup, task->getItemId());

            if (item == nullptr)
            {
                std::cout << "Robot " << robot.getId()
                          << ": Item " << task->getItemId()
                          << " not found at pickup point.\n";
            }
            else if (robot.pickUpItem(item))
            {
                std::cout << "Robot " << robot.getId()
                          << " picked up Item "
                          << task->getItemId() << ".\n";

                continueTask(robot);
            }
            else
            {
                // Pickup failed, so put the item back where it was
                warehouse.addItemAtPickup(pickup, item);
            }
        }

        // Deliver the item at delivery location
        else if (robot.isCarryingItem() &&
                 pos.getXPosition() ==
                     delivery.getXPosition() &&
                 pos.getYPosition() ==
                     delivery.getYPosition())
        {
            // Hand the item over to the delivery point
            std::shared_ptr<Item> delivered = robot.dropOffItem();
            warehouse.addItemAtDelivery(delivery, delivered);

            taskManager.markTaskCompleted(taskId);
            robot.setCurrentTaskId(-1);

            std::cout << "Robot " << robot.getId()
                      << " completed Task "
                      << taskId << ".\n";
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

bool Simulation::sendToCharge(Robot& r) {
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
        return false;
    }

    std::cout<<"Sending Robot to " << warehouse.listChargingPoints[index].ID << " charger..." << std::endl;

    warehouse.listChargingPoints[index].free = false;

    r.setRoute(path);

    return true;
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
    bool done = false;

    warehouse.displayPickupLocations();
    warehouse.displayDeliveryLocations();

    int pickupCount = static_cast<int>(warehouse.listPickupPoints.size());
    int deliveryCount = static_cast<int>(warehouse.listDeliveryPoints.size());

    if (pickupCount == 0 || deliveryCount == 0) {
        std::cout << "Need at least one pickup and one delivery point.\n";
        return;
    }

    int PUid = getInput<int>("Enter Pick Up Point ID (0 to cancel): ", 0, pickupCount);
    if (PUid == 0) return;

    int DPid = getInput<int>("Enter Delivery Point ID: ", 1, deliveryCount);


    while (!done) {
        if (!taskManager.isPickupAvailable(warehouse.listPickupPoints[PUid-1].positionPU))
        {
            std::cout << "Pickup location is already in use.\n";
            PUid = getInput<int>("Enter Pick Up Point ID (0 to cancel): ", 0, pickupCount);
            if (PUid == 0) return;
        }
        else {
            done = true;
        }
    }

    //Items come from the file, so the chosen pickup point must already have them
    const std::vector<std::shared_ptr<Item>>& items = warehouse.listPickupPoints[PUid-1].items;

    if (items.empty()) {
        std::cout << "No items at this pickup point.\n";
        return;
    }

    std::cout << "Items at pickup point " << PUid << ":\n";
    for (const auto& item : items) {
        std::cout << "  Item ID " << item->getId() << "\n";
    }

    //Keep asking until the item ID is one that is actually at this pickup point
    int itemID{};
    bool found = false;
    while (!found) {
        itemID = getInput<int>("Enter Item ID (0 to cancel): ");
        if (itemID == 0) return;

        for (const auto& item : items) {
            if (item->getId() == itemID) {
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "Item " << itemID << " is not at this pickup point.\n";
        }
    }

    taskManager.createTask(itemID, warehouse.listPickupPoints[PUid-1].positionPU, warehouse.listDeliveryPoints[DPid-1].positionD);

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
                continueTask(robot);
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

void Simulation::createWarehouseLayout()
{
    int rows = getInput<int>("Enter number of rows: ", 1, 100);
    int cols = getInput<int>("Enter number of columns: ", 1, 100);

    // Confirm before replacing the current warehouse
    int confirm = getInput<int>(
        "Replace current warehouse? (1 = Yes, 0 = No): ", 0, 1);

    if (confirm == 0) {
        std::cout << "Warehouse creation cancelled.\n";
        return;
    }

    // Create a new empty warehouse (createMap also clears all P/D/C lists)
    warehouse.createMap(rows, cols);

    // Remove robots from the previous layout
    robots.clear();

    // Clear tasks from the previous layout
    taskManager = TaskManager();

    currentTime = 0;

    // config is NOT changed, so Reset still restores the original layout from the file

    std::cout << "New warehouse layout created!\n";
    warehouse.display();
}

void Simulation::editWarehouse()
{
    int choice;

    do {
        std::cout << "\nEdit Warehouse:"
                  << "\n1. Add Wall"
                  << "\n2. Remove Wall"
                  << "\n3. Add Pickup Point"
                  << "\n4. Add Delivery Point"
                  << "\n5. Add Charging Station"
                  << "\n6. Back to Main Menu\n";

        choice = getInput<int>("Enter option: ", 1, 6);

        if (choice == 6)
            break;

        int row = getInput<int>(
            "Enter row: ", 0, warehouse.getLength() - 1);

        int col = getInput<int>(
            "Enter column: ", 0, warehouse.getWidth() - 1);

        if (choice == 1) {
            if (warehouse.getCell(row, col) == '.') {
                bool robotHere = false;
                for (const Robot& robot : robots) {
                    Position pos = robot.getPosition();
                    if (pos.getXPosition() == row &&
                        pos.getYPosition() == col) {
                        robotHere = true;
                        break;
                    }
                }

                if (robotHere) {
                    std::cout << "A robot is at this location.\n";
                }
                else if (warehouse.setCell(row, col, '#')) {
                    std::cout << "Wall added successfully.\n";
                }
            }
            else {
                std::cout << "Location is not empty.\n";
            }
        }
        else if (choice == 2) {
            if (warehouse.removeWall(row, col))
                std::cout << "Wall removed successfully.\n";
            else
                std::cout << "No wall at this location.\n";
        }
        else {
            if (warehouse.getCell(row, col) != '.') {
                std::cout << "Location is not empty.\n";
                continue;
            }

            bool robotHere = false;
            for (const Robot& robot : robots) {
                Position pos = robot.getPosition();
                if (pos.getXPosition() == row &&
                    pos.getYPosition() == col) {
                    robotHere = true;
                    break;
                }
            }

            if (robotHere) {
                std::cout << "A robot is at this location.\n";
                continue;
            }

            int id = getInput<int>("Enter point ID: ", 1, 1000000);

            if (choice == 3) {
                warehouse.setPickupPoint(row, col, id);
                std::cout << "Pickup point added.\n";
            }
            else if (choice == 4) {
                warehouse.setDeliveryPoint(row, col, id);
                std::cout << "Delivery point added.\n";
            }
            else if (choice == 5) {
                warehouse.setChargingPoint(row, col, id, true);
                std::cout << "Charging station added.\n";
            }
        }

        warehouse.display();

    } while (choice != 6);
}






