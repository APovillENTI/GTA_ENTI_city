#include "Peaton.h"
#include <cstdlib>
#include <cmath>

Peaton::Peaton(int startX, int startY, int initialHealth, int power, int island) : x(startX), y(startY), isDead(false), 
health(initialHealth), attackPower(power), islandId(island), isBeingAttacked(false)
{
    //random direction at start
    movementType = (rand() % 2 == 0) ? MovementType::HORIZONTAL : MovementType::VERTICAL;

    //50% agressive
    behavior = (rand() % 2 == 0) ? PeatonBehavior::NEUTRAL : PeatonBehavior::AGGRESSIVE;

    lastAttackTime = std::chrono::steady_clock::now();
}

bool Peaton::IsPlayerNearby(const Player& player) const
{
    int distanceX = abs(x - player.GetX());
    int distanceY = abs(y - player.GetY());

    return (distanceX <= 1 && distanceY <= 1);
}

void Peaton::Update(const Map& gameMap, const Player& player)
{
    if (isDead || IsPlayerNearby(player))
    {
        return;
    }

    Move(gameMap);
}

void Peaton::GetDamage(const int damage)
{
    health -= damage;

    if (behavior == PeatonBehavior::AGGRESSIVE && !isBeingAttacked)
    {
        isBeingAttacked = true;
        lastAttackTime = std::chrono::steady_clock::now();
    }
}

void Peaton::Kill()
{
    isDead = true;
    isBeingAttacked = false;
}

void Peaton::Move(const Map& gameMap)
{
    if (movementType == MovementType::HORIZONTAL)
    {
        int dx = (rand() % 2 == 0) ? 1 : -1;
        int newX = x + dx;

        if (gameMap.IsWalkable(newX, y))
        {
            x = newX;
        }
    }
    else
    {
        int dy = (rand() % 2 == 0) ? 1 : -1;
        int newY = y + dy;

        if (gameMap.IsWalkable(x, newY))
        {
            y = newY;
        }
    }
}

bool Peaton::CanAttack() const
{
    auto currentTime = std::chrono::steady_clock::now();
    auto timeSinceLastAttack = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastAttackTime);

    return timeSinceLastAttack.count() > 1000;
}

void Peaton::StartBeingAttacked()
{
    if (behavior == PeatonBehavior::AGGRESSIVE)
    {
        isBeingAttacked = true;
        lastAttackTime = std::chrono::steady_clock::now();
    }
}

bool Peaton::ShouldAttackPlayer() const
{
    return behavior == PeatonBehavior::AGGRESSIVE && isBeingAttacked && CanAttack();
}
