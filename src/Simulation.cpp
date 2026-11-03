#include "../include/Simulation.h"
#include "SimulationConfig.h"

#include <iostream>
#include <limits>
#include <string>

Simulation::Simulation(const SimulationConfig& config) {

    this->config = config;
    buildFromConfig();


} //initialize robot: ID, position. Warehouse: length and width


void Simulation::buildFromConfig() {
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
        "\n9.Exit" << std::endl;

        option = getInput<int> ("Enter Option: ", 1, 9);


        switch(option){
            case 1:
                warehouse.display();
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                buildFromConfig();
                std::cout<<"Warehouse was reset to initial settings";
                break;
            case 8:
                break;
            case 9:
                done = true;
                break;
        }
    }

}

void Simulation::advanceTime() {//updates everything
    currentTime++;
    //need robotManager
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

Simulation::~Simulation(){}


