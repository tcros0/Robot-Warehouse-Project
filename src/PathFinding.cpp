
#include "../include/PathFinding.h"
#include <queue>
#include <algorithm>
#include "Warehouse.h"

PathFinding::PathFinding() {}
PathFinding::~PathFinding() {}

// Check if a position is inside the warehouse and not a wall
bool PathFinding::isWalkable(
    const Warehouse& warehouse,
    const Position& position) const
{
    int row = position.getXPosition();
    int col = position.getYPosition();

    // Check boundaries before accessing the cell
    if (!warehouse.isInBounds(row, col))
        return false;

    return warehouse.getCell(row, col) != '#';
}

// Breadth First Search
std::vector<Position> PathFinding::findPath(
    const Warehouse& warehouse,
    const Position& start,
    const Position& goal) const
{
    int rows = warehouse.getLength();
    int cols = warehouse.getWidth();

    // Start and goal must be valid
    if (!isWalkable(warehouse, start) ||
        !isWalkable(warehouse, goal))
        return {};

    std::vector<std::vector<bool>> visited(
        rows, std::vector<bool>(cols, false));

    std::vector<std::vector<Position>> cameFrom(
        rows, std::vector<Position>(
            cols, Position(-1, -1)));

    std::queue<Position> toVisit;
    toVisit.push(start);

    visited[start.getXPosition()][start.getYPosition()] = true;

    // Up, down, left, right
    const int dRow[4] = {-1, 1, 0, 0};
    const int dCol[4] = {0, 0, -1, 1};

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
            Position next(
                current.getXPosition() + dRow[i],
                current.getYPosition() + dCol[i]);

            if (!isWalkable(warehouse, next))
                continue;

            int r = next.getXPosition();
            int c = next.getYPosition();

            if (visited[r][c])
                continue;

            visited[r][c] = true;
            cameFrom[r][c] = current;
            toVisit.push(next);
        }
    }

    if (!found)
        return {};

    // Reconstruct path
    std::vector<Position> path;
    Position step = goal;

    while (!(step.getXPosition() == start.getXPosition() &&
             step.getYPosition() == start.getYPosition()))
    {
        path.push_back(step);
        step = cameFrom[step.getXPosition()][step.getYPosition()];
    }

    std::reverse(path.begin(), path.end());
    return path;
}

// Find a path while avoiding other robots
std::vector<Position> PathFinding::findPathAvoidingRobots(
    const Warehouse& warehouse,
    const Position& start,
    const Position& goal,
    const std::vector<Robot>& robots) const
{
    int rows = warehouse.getLength();
    int cols = warehouse.getWidth();

    // Start and goal must be valid
    if (!isWalkable(warehouse, start) ||
        !isWalkable(warehouse, goal))
        return {};

    // Check if another robot occupies the goal
    for (const Robot& robot : robots)
    {
        Position pos = robot.getPosition();

        if (pos.getXPosition() == goal.getXPosition() &&
            pos.getYPosition() == goal.getYPosition() &&
            !(pos.getXPosition() == start.getXPosition() &&
              pos.getYPosition() == start.getYPosition()))
        {
            return {};
        }
    }

    std::vector<std::vector<bool>> visited(
        rows, std::vector<bool>(cols, false));

    std::vector<std::vector<Position>> cameFrom(
        rows, std::vector<Position>(
            cols, Position(-1, -1)));

    std::queue<Position> toVisit;
    toVisit.push(start);

    visited[start.getXPosition()][start.getYPosition()] = true;

    // Up, down, left, right
    const int dRow[4] = {-1, 1, 0, 0};
    const int dCol[4] = {0, 0, -1, 1};

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
            Position next(
                current.getXPosition() + dRow[i],
                current.getYPosition() + dCol[i]);

            if (!isWalkable(warehouse, next))
                continue;

            int r = next.getXPosition();
            int c = next.getYPosition();

            if (visited[r][c])
                continue;

            // Check if another robot occupies this cell
            bool occupied = false;

            for (const Robot& robot : robots)
            {
                Position pos = robot.getPosition();

                if (pos.getXPosition() == r &&
                    pos.getYPosition() == c &&
                    !(r == start.getXPosition() &&
                      c == start.getYPosition()))
                {
                    occupied = true;
                    break;
                }
            }

            if (occupied)
                continue;

            visited[r][c] = true;
            cameFrom[r][c] = current;
            toVisit.push(next);
        }
    }

    if (!found)
        return {};

    // Reconstruct alternative path
    std::vector<Position> newPath;
    Position step = goal;

    while (!(step.getXPosition() == start.getXPosition() &&
             step.getYPosition() == start.getYPosition()))
    {
        newPath.push_back(step);
        step = cameFrom[step.getXPosition()][step.getYPosition()];
    }

    std::reverse(newPath.begin(), newPath.end());
    return newPath;
}

// Check whether a cell is free of robots
bool PathFinding::freeSpace(
    RobotManager robots,
    Position position)
{
    for (const Robot& robot : robots.getRobots())
    {
        Position robotPos = robot.getPosition();

        if (robotPos.getXPosition() == position.getXPosition() &&
            robotPos.getYPosition() == position.getYPosition())
        {
            return false;
        }
    }

    return true;
}
