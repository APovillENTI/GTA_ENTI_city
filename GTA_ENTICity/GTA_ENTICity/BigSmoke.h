#pragma once
#include <chrono>
#include "Map.h"
#include "Player.h"

class BigSmoke
{
private:
    int x;
    int y;
    int health;
    int attackPower;
    bool isDead;
    std::chrono::steady_clock::time_point lastAttackTime;

    bool IsPlayerNearby(const Player& player) const;
    bool CanAttack() const;

public:
    BigSmoke(int startX, int startY, int initialHealth, int power);

    // Getters
    int GetX() const { return x; }
    int GetY() const { return y; }
    bool IsDead() const { return isDead; }
    int GetHP() const { return health; }
    int GetPower() const { return attackPower; }

    void Update(const Map& gameMap, const Player& player);
    void Kill();
    void GetDamage(const int damage);
    bool ShouldAttackPlayer(const Player& player) const;
    void Move(const Map& gameMap);
};