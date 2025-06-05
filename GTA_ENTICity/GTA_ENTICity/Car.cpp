#include "Car.h"

Car::Car(int startX, int startY, int island) : x(startX), y(startY), islandId(island), isBeingDriven(false){}

bool Car::Move(int dx, int dy, const Map& gameMap)
{
    int newX = x + dx;
    int newY = y + dy;

    if (gameMap.IsWalkable(newX, newY))
    {
        x = newX;
        y = newY;
        return true;
    }

    return false;
}