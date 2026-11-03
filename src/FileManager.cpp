// Updated by farrokh880 11/01/2026

#include "../include/FileManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<Position> FileManager::readPositions(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    std::vector<Position> result;
    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        ++lineNum;
        if (line.find_first_not_of(" \t\r") == std::string::npos) continue;

        std::istringstream iss(line);
        int x, y;
        if (!(iss >> x >> y)) {
            throw std::runtime_error("Bad format in " + filename +
                                     " at line " + std::to_string(lineNum));
        }
        result.push_back(Position(x, y));
    }
    return result;
}

void FileManager::loadPickups(const std::string& filename) {
    pickups_ = readPositions(filename);
}

void FileManager::loadDeliveries(const std::string& filename) {
    deliveries_ = readPositions(filename);
}

const std::vector<Position>& FileManager::getPickups() const { return pickups_; }
const std::vector<Position>& FileManager::getDeliveries() const { return deliveries_; }

size_t FileManager::pickupCount() const { return pickups_.size(); }
size_t FileManager::deliveryCount() const { return deliveries_.size(); }
