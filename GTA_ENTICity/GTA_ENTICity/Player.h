#pragma once
#include "Map.h"

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Car;

class Player
{
private:
    int x;
    int y;
    Direction direction;
    int money;
    Car* currentCar; // Pointer to the car being driven
    bool isDriving;

public:
    Player();
    Player(int startX, int startY, Direction startDir);

    // Getters
    int GetX() const { return x; }
    int GetY() const { return y; }
    Direction GetDirection() const { return direction; }
    int GetMoney() const { return money; }
    bool IsDriving() const { return isDriving; }
    Car* GetCurrentCar() const { return currentCar; }

    // Display
    char GetDirectionChar() const;

    // Setters
    void SetPosition(int newX, int newY);
    void SetDirection(Direction newDir);
    void AddMoney(int amount);

    // Movement
    bool Move(int dx, int dy, const Map& gameMap);
    bool IsAdjacentTo(int targetX, int targetY) const;

    // Car control
    void EnterCar(Car* car);
    void ExitCar(const Map& gameMap);
};