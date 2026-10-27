
#pragma once

#include <string>

// Types of items
enum class StorageType 
{
    Dry,
    Refrigerated,
    Frozen
};

// Base class for items
class Item 
{
protected:
    // Item information
    int id;
    std::string name;

public:
    // Create an item
    Item(int itemId, const std::string& itemName);

    // Virtual destructor
    virtual ~Item();

    // Get item information
    int getId() const;
    std::string getName() const;

    // Get item type
    virtual StorageType getType() const = 0;

    // Get battery drain multiplier
    virtual double getDrainMultiplier() const = 0;
};

