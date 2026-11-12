

#include "Item.h"

Item::Item(int itemId, const std::string& itemName)
    : id(itemId), name(itemName)
{
}

Item::~Item()
{
}

int Item::getId() const
{
    return id;
}

std::string Item::getName() const
{
    return name;
}
