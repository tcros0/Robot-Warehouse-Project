#ifndef SIMULATIONCONFIG_H
#define SIMULATIONCONFIG_H

#include <vector>
#include "Warehouse.h"
#include "Position.h"

struct RobotConfig {

    int ID{};
    Position position;
    int battery{};
};

struct SimulationConfig {

    int length{};
    int width{};

    std::vector<deliveryPoint> deliveryPoints;
    std::vector<pickupPoint> pickupPoints;
    std::vector<chargingPoint> chargingPoints;
    std::vector<Position> obstacles;

    std::vector<RobotConfig> robots;
};


#endif //SIMULATIONCONFIG_H
