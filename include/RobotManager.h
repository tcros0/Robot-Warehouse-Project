#ifndef WAREHOUSEROBOTSIM_ROBOTMANAGER_H
#define WAREHOUSEROBOTSIM_ROBOTMANAGER_H

#include <vector>
#include "Robot.h"

class RobotManager {
private:
    std::vector<Robot> robots_;
public:
    void addRobot(const Robot& robot);
    void addRobot(int ID, Position position, int width, int height, int battery);
    void updateAll();
    Robot* findById(int id);
    const Robot* findById(int id) const;
    const std::vector<Robot>& getRobots() const;
    size_t robotCount() const;
};
#endif 

