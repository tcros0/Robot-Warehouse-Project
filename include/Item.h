
#pragma once

#include <string>

// Item class
class Item
{
private:
    int id;
    std::string name;

public:
    // Create an item
    Item(int itemId, const std::string& itemName);

    // Get item information
    int getId() const;
    std::string getName() const;
};

