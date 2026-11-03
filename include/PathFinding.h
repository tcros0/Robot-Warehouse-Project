#pragma once
#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <vector>
#include "Position.h"
#include "Warehouse.h"
#include "RobotManager.h"

class PathFinding {
private:
    std::vector<Position> path;

public:
    PathFinding();
    std::vector<Position> findPath(const Warehouse& warehouse, const Position& start, const Position& goal) const;
    bool isWalkable(const Warehouse& warehouse, const Position& position) const;
    bool freeSpace(RobotManager robots, Position position);
    ~PathFinding();
};


#endif //PATHFINDING_H
