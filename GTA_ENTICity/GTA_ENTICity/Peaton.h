#pragma once
#include "Map.h"
#include "Player.h"
//same functions as WALKER.h, diferent name

enum class MovementType
{
    HORIZONTAL,
    VERTICAL
};

class Peaton
{
private:
    int x;
    int y;
    int health;
    int attackPower;
    bool isDead;
    MovementType movementType;
    int islandId;

    bool IsPlayerNearby(const Player& player) const;

public:
    Peaton(int startX, int startY, int initialHealth, int power, int island);

    // Getters
    int GetX() const { return x; }
    int GetY() const { return y; }
    bool IsDead() const { return isDead; }
    int GetHP() const { return health; }
    int GetPower() const { return attackPower; }
    int GetIslandId() const { return islandId; }

    // Métodos principales
    void Update(const Map& gameMap, const Player& player);
    void Kill();
    void GetDamage(const int damage);

    // Métodos de movimiento
    void Move(const Map& gameMap);
};