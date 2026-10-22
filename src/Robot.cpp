//
bool Robot::moveUp() {
    if (batteryLevel <= 0 || position.row <= 0)
        return false;

    position.row--;
    drainBattery(1);
    return true;
}

bool Robot::moveDown() {
    if (batteryLevel <= 0 || position.row >= 14)
        return false;

    position.row++;
    drainBattery(1);
    return true;
}

bool Robot::moveLeft() {
    if (batteryLevel <= 0 || position.col <= 0)
        return false;

    position.col--;
    drainBattery(1);
    return true;
}

bool Robot::moveRight() {
    if (batteryLevel <= 0 || position.col >= 24)
        return false;

    position.col++;
    drainBattery(1);
    return true;
}
 Robot implementation
