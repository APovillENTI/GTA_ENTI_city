#pragma once
#include <chrono>
#include "Map.h"
#include "Player.h"
//same functions as WALKER.h, diferent name

enum class MovementType
{
    HORIZONTAL,
    VERTICAL
};

enum class PeatonBehavior
{
    NEUTRAL,
    AGGRESSIVE
};


class Peaton
{
private:
    int x;
    int y;
    int health;
    int attackPower;
    bool isDead;
    int islandId;
    MovementType movementType;
    PeatonBehavior behavior;
    std::chrono::steady_clock::time_point lastAttackTime;
    bool isBeingAttacked;

    bool IsPlayerNearby(const Player& player) const;
    bool CanAttack() const;

public:
    Peaton(int startX, int startY, int initialHealth, int power, int island);

    // Getters
    int GetX() const { return x; }
    int GetY() const { return y; }
    bool IsDead() const { return isDead; }
    int GetHP() const { return health; }
    int GetPower() const { return attackPower; }
    int GetIslandId() const { return islandId; }
    PeatonBehavior GetBehavior() const { return behavior; }

    // main methods
    void Update(const Map& gameMap, const Player& player);
    void Kill();
    void GetDamage(const int damage);
    void StartBeingAttacked();
    bool ShouldAttackPlayer() const;
    void Move(const Map& gameMap);
};