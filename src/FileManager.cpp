// Updated by farrokh880 11/01/2026

#include "../include/FileManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

//Reads the whole warehouse setup from one file. Throws std::runtime_error on any problem
SimulationConfig FileManager::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    SimulationConfig config;
    bool sizeFound = false;

    //Items are attached to their pickup point after the whole file is read
    struct PendingItem {
        int pickupID;
        int itemID;
        int lineNum;
    };
    std::vector<PendingItem> pendingItems;

    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        ++lineNum;

        //Skip blank lines
        size_t first = line.find_first_not_of(" \t\r");
        if (first == std::string::npos) continue;

        //Skip comment lines starting with //
        if (line.compare(first, 2, "//") == 0) continue;

        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        //Used in every error message so the user knows exactly which line is wrong
        std::string where = " in " + filename + " at line " + std::to_string(lineNum);

        if (keyword == "SIZE") {
            int length, width;
            if (!(iss >> length >> width) || length <= 0 || width <= 0) {
                throw std::runtime_error("Bad SIZE (expected: SIZE rows columns)" + where);
            }
            config.length = length;
            config.width = width;
            sizeFound = true;
        }
        else if (keyword == "PICKUP") {
            int id, x, y;
            if (!(iss >> id >> x >> y)) {
                throw std::runtime_error("Bad PICKUP (expected: PICKUP ID row col)" + where);
            }
            pickupPoint p;
            p.ID = id;
            p.positionPU = Position(x, y);
            config.pickupPoints.push_back(p);
        }
        else if (keyword == "DELIVERY") {
            int id, x, y;
            if (!(iss >> id >> x >> y)) {
                throw std::runtime_error("Bad DELIVERY (expected: DELIVERY ID row col)" + where);
            }
            deliveryPoint d;
            d.ID = id;
            d.positionD = Position(x, y);
            config.deliveryPoints.push_back(d);
        }
        else if (keyword == "CHARGER") {
            int id, x, y;
            if (!(iss >> id >> x >> y)) {
                throw std::runtime_error("Bad CHARGER (expected: CHARGER ID row col)" + where);
            }
            chargingPoint c;
            c.ID = id;
            c.positionC = Position(x, y);
            c.free = true;   //every charger starts free
            config.chargingPoints.push_back(c);
        }
        else if (keyword == "WALL") {
            int x, y;
            if (!(iss >> x >> y)) {
                throw std::runtime_error("Bad WALL (expected: WALL row col)" + where);
            }
            config.obstacles.push_back(Position(x, y));
        }
        else if (keyword == "ROBOT") {
            int id, x, y, battery;
            if (!(iss >> id >> x >> y >> battery) || battery < 0 || battery > 100) {
                throw std::runtime_error("Bad ROBOT (expected: ROBOT ID row col battery 0-100)" + where);
            }
            RobotConfig rc;
            rc.ID = id;
            rc.position = Position(x, y);
            rc.battery = battery;
            config.robots.push_back(rc);
        }
        else if (keyword == "ITEM") {
            int pickupID, itemID;
            if (!(iss >> pickupID >> itemID)) {
                throw std::runtime_error("Bad ITEM (expected: ITEM pickupID itemID)" + where);
            }
            pendingItems.push_back({pickupID, itemID, lineNum});
        }
        else {
            throw std::runtime_error("Unknown keyword '" + keyword + "'" + where);
        }
    }

    if (!sizeFound) {
        throw std::runtime_error("No SIZE line in " + filename);
    }

    //Attach each item to the pickup point with the matching ID
    for (const PendingItem& pi : pendingItems) {
        bool attached = false;
        for (pickupPoint& p : config.pickupPoints) {
            if (p.ID == pi.pickupID) {
                //Item still needs a name, so a default one is generated from the ID
                p.items.push_back(std::make_shared<Item>(pi.itemID, "Item " + std::to_string(pi.itemID)));
                attached = true;
                break;
            }
        }
        if (!attached) {
            throw std::runtime_error("ITEM refers to unknown pickup ID " + std::to_string(pi.pickupID) +
                                     " in " + filename + " at line " + std::to_string(pi.lineNum));
        }
    }

    return config;
}

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

//Writes the whole setup in the same format loadConfig reads, so a saved file can be loaded again
void FileManager::saveConfig(const SimulationConfig& config, const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot write file: " + filename);
    }

    file << "// Warehouse layout (x = row, y = column)\n";
    file << "SIZE " << config.length << " " << config.width << "\n\n";

    file << "// PICKUP ID row col\n";
    for (const pickupPoint& p : config.pickupPoints) {
        file << "PICKUP " << p.ID << " " << p.positionPU.getXPosition() << " " << p.positionPU.getYPosition() << "\n";
    }

    file << "\n// DELIVERY ID row col\n";
    for (const deliveryPoint& d : config.deliveryPoints) {
        file << "DELIVERY " << d.ID << " " << d.positionD.getXPosition() << " " << d.positionD.getYPosition() << "\n";
    }

    file << "\n// CHARGER ID row col\n";
    for (const chargingPoint& c : config.chargingPoints) {
        file << "CHARGER " << c.ID << " " << c.positionC.getXPosition() << " " << c.positionC.getYPosition() << "\n";
    }

    file << "\n// WALL row col\n";
    for (const Position& w : config.obstacles) {
        file << "WALL " << w.getXPosition() << " " << w.getYPosition() << "\n";
    }

    file << "\n// ROBOT ID row col battery\n";
    for (const RobotConfig& r : config.robots) {
        file << "ROBOT " << r.ID << " " << r.position.getXPosition() << " " << r.position.getYPosition() << " " << r.battery << "\n";
    }

    file << "\n// ITEM pickupID itemID\n";
    for (const pickupPoint& p : config.pickupPoints) {
        for (const auto& item : p.items) {
            file << "ITEM " << p.ID << " " << item->getId() << "\n";
        }
    }
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