#include "BigSmoke.h"
#include <cstdlib>
#include <cmath>

BigSmoke::BigSmoke(int startX, int startY, int initialHealth, int power) : x(startX), y(startY), isDead(false), health(initialHealth), attackPower(power)
{
    lastAttackTime = std::chrono::steady_clock::now();
}

bool BigSmoke::IsPlayerNearby(const Player& player) const
{
    int distanceX = abs(x - player.GetX());
    int distanceY = abs(y - player.GetY());

    return (distanceX <= 1 && distanceY <= 1);
}

void BigSmoke::Update(const Map& gameMap, const Player& player)
{
    if (isDead || IsPlayerNearby(player))
    {
        return;
    }

    Move(gameMap);
}

void BigSmoke::GetDamage(const int damage)
{
    health -= damage;
    lastAttackTime = std::chrono::steady_clock::now();
}

void BigSmoke::Kill()
{
    isDead = true;
}

void BigSmoke::Move(const Map& gameMap)
{
    int dx = 0, dy = 0;

    if (rand() % 2 == 0) // 50% horizontal movement
    {
        dx = (rand() % 2 == 0) ? 1 : -1;
    }
    else // 50% vertical movement
    {
        dy = (rand() % 2 == 0) ? 1 : -1;
    }

    int newX = x + dx;
    int newY = y + dy;

    if (gameMap.IsWalkable(newX, newY))
    {
        x = newX;
        y = newY;
    }
}

bool BigSmoke::CanAttack() const
{
    auto currentTime = std::chrono::steady_clock::now();
    auto timeSinceLastAttack = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastAttackTime);

    return timeSinceLastAttack.count() > 1000;
}

bool BigSmoke::ShouldAttackPlayer(const Player& player) const
{
    return !isDead && IsPlayerNearby(player) && CanAttack();
}