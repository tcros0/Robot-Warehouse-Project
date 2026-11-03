
bool TaskManager::isPickupAvailable(Position pickup) const
{
    // Check active tasks at this pickup location
    for (const Task& task : tasks)
    {
        Position location = task.getPickupLocation();

        if (location.getXPosition() == pickup.getXPosition() &&
            location.getYPosition() == pickup.getYPosition() &&
            task.getStatus() != "Completed")
        {
            return false;
        }
    }

    return true;
}

