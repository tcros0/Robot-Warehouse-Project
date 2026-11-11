#include "../include/FileManager.h"
#include "../include/Simulation.h"
#include "../include/SimulationConfig.h"

#include <iostream>
#include <stdexcept>

int main() {
    FileManager fileManager;
    SimulationConfig config;

    try {
        config = fileManager.loadConfig("data/Warehouse.txt");
    }
    catch (const std::runtime_error& e) {
        std::cout << "Error loading warehouse: " << e.what() << std::endl;
        return 1;
    }

    Simulation sim(config);
    sim.run();

    return 0;
}