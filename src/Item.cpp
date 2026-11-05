
#include "Item.h"

// Create an item
Item::Item(int itemId, const std::string& itemName)
    : id(itemId), name(itemName)
{
}

// Virtual destructor
Item::~Item()
{
}

// Get item ID
int Item::getId() const
{
    return id;
}

// Get item name
std::string Item::getName() const
{
    return name;
}
