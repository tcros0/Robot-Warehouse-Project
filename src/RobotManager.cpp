#include "../include/RobotManager.h"
#include <stdexcept>



void RobotManager::addRobot(const Robot& robot) {
    if (findById(robot.getId()) != nullptr) {
        throw std::invalid_argument("Duplicate robot ID: " + std::to_string(robot.getId()));
    }
    robots_.push_back(robot);
}

void RobotManager::updateAll() {
    for (auto& robot : robots_) {
        if (robot.getBatteryLevel() <= 0) continue;  // dead robots stay put
        
    }
}
Robot* RobotManager::findById(int id) {
    for (auto& robot : robots_) {
        if (robot.getId() == id) return &robot;
    }
    return nullptr;
}
const Robot* RobotManager::findById(int id) const {
    for (const auto& robot : robots_) {
        if (robot.getId() == id) return &robot;
    }
    return nullptr;
}


const std::vector<Robot>& RobotManager::getRobots() const { return robots_; }
size_t RobotManager::robotCount() const { return robots_.size(); }