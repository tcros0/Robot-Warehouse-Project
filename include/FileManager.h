//
// Created by Plokiu on 10/22/2026.
// Updated bu Farrokh880 10/31/2026

#ifndef WAREHOUSEROBOTSIM_FILEMANAGER_H
#define WAREHOUSEROBOTSIM_FILEMANAGER_H

#include <string>
#include <vector>
#include "Position.h"

class FileManager {
private:
    std::vector<Position> pickups_;
    std::vector<Position> deliveries_;

    static std::vector<Position> readPositions(const std::string& filename);

public:
    void loadPickups(const std::string& filename);
    void loadDeliveries(const std::string& filename);

    const std::vector<Position>& getPickups() const;
    const std::vector<Position>& getDeliveries() const;

    size_t pickupCount() const;
    size_t deliveryCount() const;
};

#endif 
