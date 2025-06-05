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
    int health;
    int attackPower;
    Car* currentCar; // Pointer to the car being driven
    bool isDriving;
    bool inSanFiero;

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
    int GetHealth() const { return health; }
    int GetPower() const { return attackPower; }
    bool InSF() const { return inSanFiero; }

    // Display
    char GetDirectionChar() const;

    // Setters
    void SetHealth(int newHealth);
    void SetPower(int newPwr);
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