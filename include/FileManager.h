#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "Position.h"
#include "SimulationConfig.h"

class FileManager {
public:
    //Reads the whole warehouse setup (size, points, walls, robots) from one file
    SimulationConfig loadConfig(const std::string& filename);
    void saveConfig(const SimulationConfig& config, const std::string& filename) const;

    //Original functions (Farrokh)
    void loadPickups(const std::string& filename);
    void loadDeliveries(const std::string& filename);

    const std::vector<Position>& getPickups() const;
    const std::vector<Position>& getDeliveries() const;

    size_t pickupCount() const;
    size_t deliveryCount() const;

private:
    std::vector<Position> readPositions(const std::string& filename);

    std::vector<Position> pickups_;
    std::vector<Position> deliveries_;
};

#endif //FILEMANAGER_H