#include "../include/PathFinding.h"
#include <queue>
#include <algorithm>
#include "Warehouse.h"

PathFinding::PathFinding() {};
PathFinding::~PathFinding() {};

bool PathFinding::isWalkable(const Warehouse& warehouse, const Position& position) const{
    char bound  = warehouse.getCell(position.getXPosition(), position.getYPosition());
    if (!warehouse.isInBounds(position.getXPosition(), position.getYPosition())) {
        return false;
    }
    else if (bound == '#') {
        return false;
    }
    else {
        return true;
    }
}

//Beardth first search
std::vector<Position> PathFinding::findPath(const Warehouse& warehouse, const Position& start, const Position& goal) const{
    int rows = warehouse.getLength();   // friend access
    int cols = warehouse.getWidth();

    // goal must be a walkable cell
    if (!isWalkable(warehouse, goal))
    {
        return {};
    }

    // visited[row][col] = has BFS reached this cell yet?
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    // cameFrom[row][col] = the cell we stepped from to reach this one
    std::vector<std::vector<Position>> cameFrom(rows, std::vector<Position>(cols, Position(-1, -1)));

    std::queue<Position> toVisit;
    toVisit.push(start);
    visited[start.getXPosition()][start.getYPosition()] = true;

    // the 4 directions: up, down, left, right (as row change, column change)
    const int dRow[4] = { -1, 1, 0, 0 };
    const int dCol[4] = { 0, 0, -1, 1 };

    bool found = false;

    while (!toVisit.empty())
    {
        Position current = toVisit.front();
        toVisit.pop();

        if (current.getXPosition() == goal.getXPosition() &&
            current.getYPosition() == goal.getYPosition())
        {
            found = true;
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            Position next(current.getXPosition() + dRow[i],
                          current.getYPosition() + dCol[i]);

            if (!isWalkable(warehouse, next))
            {
                continue;   // wall or off the map
            }

            int r = next.getXPosition();
            int c = next.getYPosition();

            if (visited[r][c])
            {
                continue;   // already reached by a shorter or equal route
            }

            visited[r][c] = true;
            cameFrom[r][c] = current;
            toVisit.push(next);
        }
    }

    if (!found)
    {
        return {};   // unreachable
    }

    // walk backward from the goal to the start
    std::vector<Position> path;
    Position step = goal;
    while (!(step.getXPosition() == start.getXPosition() &&
             step.getYPosition() == start.getYPosition()))
    {
        path.push_back(step);
        step = cameFrom[step.getXPosition()][step.getYPosition()];
    }

    reverse(path.begin(), path.end());   // now start -> goal
    return path;
}