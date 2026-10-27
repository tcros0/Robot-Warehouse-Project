#include<vector>

#pragma once

#include <string>

class Item {
private:
    int id;
    std::string name;

public:
    Item(int itemId, const std::string& itemName);

    int getId() const;
    std::string getName() const;
};

