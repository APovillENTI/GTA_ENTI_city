#pragma once
#include "Map.h"

class Car
{
private:
    int x;
    int y;
    int islandId;
    bool isBeingDriven;

public:
    Car(int startX, int startY, int island);

    // Getters
    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetIslandId() const { return islandId; }
    bool IsBeingDriven() const { return isBeingDriven; }

    // Movement
    bool Move(int dx, int dy, const Map& gameMap);

    // Control
    void SetBeingDriven(bool driven) { isBeingDriven = driven; }
    void SetPosition(int newX, int newY) { x = newX; y = newY; }
};