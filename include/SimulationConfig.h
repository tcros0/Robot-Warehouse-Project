#ifndef SIMULATIONCONFIG_H
#define SIMULATIONCONFIG_H

#include <vector>

#include "Position.h"

struct RobotConfig {

    int ID{};
    Position position;
    int battery{};
};

struct SimulationConfig {

    int length{};
    int width{};
    std::vector<RobotConfig> robots;
};


#endif //SIMULATIONCONFIG_H
