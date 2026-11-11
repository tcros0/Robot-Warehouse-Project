//
// Created by Plokiu on 10/22/2026.
// Updated by Farrokh880 11/03/2026

#ifndef WAREHOUSEROBOTSIM_ROBOTMANAGER_H
#define WAREHOUSEROBOTSIM_ROBOTMANAGER_H

#include <vector>
#include "Robot.h"

class RobotManager {
private:
    std::vector<Robot> robots_;
public:
    void addRobot(const Robot& robot);
    void updateAll();
    Robot* findById(int id);
    const Robot* findById(int id) const;
    const std::vector<Robot>& getRobots() const;
    size_t robotCount() const;
};
#endif 

