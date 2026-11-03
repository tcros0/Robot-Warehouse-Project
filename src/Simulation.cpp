#include "../include/Simulation.h"
#include "SimulationConfig.h"

#include <iostream>
#include <limits>
#include <string>

Simulation::Simulation(const SimulationConfig& config) {

    warehouse.setLength(config.length);
    warehouse.setWidth(config.width);

    for (const auto& rc : config.robots) {
        robots.emplace_back(rc.ID, rc.position, rc.battery);
    }

} //initialize robot: ID, position. Warehouse: length and width

void Simulation::run() {

    int option{};

    std::cout<<"Warehouse Manager:"
    "\n1.See warehouse map"
    "\n2.Check robot status"
    "\n3.Create Task"
    "\n4.View Task list"
    "\n5.Create new Warehouse Layout"
    "\n6.Edit Warehouse"
    "\n7.Advance time"
    "\n8.Exit"
    "\nEnter option: ";
    std::cin>>option;
}

//grabs an input and validates it using getInput<dataType>()
template <typename T>
T getInput(const std::string& prompt) {
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
T getInput(const std::string& prompt, T min, T max) {
    while (true) {
        T value = getInput<T>(prompt);
        if (value >= min && value <= max) return value;
        std::cout << "Please enter a value from " << min << " to " << max << ".\n";
    }
}
Simulation::~Simulation(){};


