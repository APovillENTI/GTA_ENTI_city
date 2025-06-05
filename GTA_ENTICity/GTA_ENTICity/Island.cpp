#include "Island.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

Island::Island(int id, int numPeatones, int maxMoneyAmount, int maxPedestrianHealth, int pedestrianPower) : islandId(id), maxMoney(maxMoneyAmount), 
pedestrianHealth(maxPedestrianHealth), pedestrianAttackPower(pedestrianPower), numCars(0)
{
    peatones.reserve(numPeatones);
    moneyDrops.reserve(20);
}

Island::Island(int id, int numPeatones, int maxMoneyAmount, int maxPedestrianHealth, int pedestrianPower, int numCarAmount) : islandId(id), maxMoney(maxMoneyAmount),
pedestrianHealth(maxPedestrianHealth), pedestrianAttackPower(pedestrianPower), numCars(numCarAmount)
{
    peatones.reserve(numPeatones);
    cars.reserve(numCars);
    moneyDrops.reserve(20);
}

void Island::GenerateRandomPosition(int mapWidth, int mapHeight, int& x, int& y) const
{
    // Generate position within island bounds
    int islandWidth = mapWidth / 3;
    int startX = islandId * islandWidth + 1;
    int endX = startX + islandWidth - 2;

    x = startX + (rand() % (endX - startX + 1));
    y = 1 + (rand() % (mapHeight - 2));
}

void Island::GenerateInitialPeatones(int mapWidth, int mapHeight)
{
    for (int i = 0; i < peatones.capacity(); i++)
    {
        int x, y;
        GenerateRandomPosition(mapWidth, mapHeight, x, y);
        peatones.emplace_back(x, y, pedestrianHealth, pedestrianAttackPower, islandId);
    }
}

void Island::GenerateInitialCars(int mapWidth, int mapHeight)
{
    for (int i = 0; i < numCars; i++)
    {
        int x, y;
        bool validPosition = false;
        int attempts = 0;

        while (!validPosition && attempts < 100)
        {
            GenerateRandomPosition(mapWidth, mapHeight, x, y);

            validPosition = true;

            // Check if position conflicts with existing cars
            for (const auto& car : cars)
            {
                if (car.GetX() == x && car.GetY() == y)
                {
                    validPosition = false;
                    break;
                }
            }

            // Check if position conflicts with peatones
            if (validPosition)
            {
                for (const auto& peaton : peatones)
                {
                    if (peaton.GetX() == x && peaton.GetY() == y)
                    {
                        validPosition = false;
                        break;
                    }
                }
            }

            attempts++;
        }

        if (validPosition)
        {
            cars.emplace_back(x, y, islandId);
        }
    }
}

void Island::UpdatePeatones(const Map& gameMap, const Player& player)
{
    for (auto& peaton : peatones)
    {
        if (!peaton.IsDead())
        {
            peaton.Update(gameMap, player);
        }
    }
}

void Island::ProcessPlayerAttack(const Player& player, const Map& gameMap)
{
    if (player.IsDriving())
        return;

    for (auto& peaton : peatones)
    {
        if (!peaton.IsDead() && player.IsAdjacentTo(peaton.GetX(), peaton.GetY()))
        {
            peaton.GetDamage(player.GetPower());
            if (peaton.GetHP() <= 0)
            {
                peaton.Kill();
                DropMoney(peaton.GetX(), peaton.GetY(), gameMap);
            }
            break; //break so we dont hit 2 peatones at a time
        }
    }
}

void Island::ProcessCarHitPeaton(const Player& player, const Map& gameMap)
{
    if (!player.IsDriving() || player.GetCurrentCar() == nullptr)
        return;

    Car* currentCar = player.GetCurrentCar();

    for (auto& peaton : peatones)
    {
        if (!peaton.IsDead() && peaton.GetX() == currentCar->GetX() && peaton.GetY() == currentCar->GetY())
        {
            peaton.Kill();
            DropMoney(peaton.GetX(), peaton.GetY(), gameMap);
        }
    }
}

void Island::DropMoney(int x, int y, const Map& gameMap)
{
    // Try to find a nearby free position for money
    int dx[] = { 0, -1, 1, 0, 0, -1, -1, 1, 1 };
    int dy[] = { 0, 0, 0, -1, 1, -1, 1, -1, 1 }; // 9 directions (including diagonals)

    for (int i = 0; i < 9; i++)
    {
        int newX = x + dx[i];
        int newY = y + dy[i];

        if (gameMap.IsWalkable(newX, newY))
        {
            // Check if there's already money at this position
            bool hasMoneyHere = false;
            for (const auto& money : moneyDrops)
            {
                if (money.x == newX && money.y == newY)
                {
                    hasMoneyHere = true;
                    break;
                }
            }

            if (!hasMoneyHere)
            {
                int amount = 1 + (rand() % maxMoney);
                moneyDrops.emplace_back(newX, newY, amount);
                break;
            }
        }
    }
}

void Island::ProcessMoneyCollection(Player& player)
{
    // Only collect money if player is not driving
    if (player.IsDriving())
        return;

    for (auto it = moneyDrops.begin(); it != moneyDrops.end();)
    {
        if (it->x == player.GetX() && it->y == player.GetY())
        {
            player.AddMoney(it->amount);
            it = moneyDrops.erase(it);
        }
        else
        {
            it++;
        }
    }
}

bool Island::HasPeatonAt(int x, int y) const
{
    for (const auto& peaton : peatones)
    {
        if (!peaton.IsDead() && peaton.GetX() == x && peaton.GetY() == y)
        {
            return true;
        }
    }
    return false;
}

bool Island::HasMoneyAt(int x, int y) const
{
    for (const auto& money : moneyDrops)
    {
        if (money.x == x && money.y == y)
        {
            return true;
        }
    }
    return false;
}

bool Island::HasCarAt(int x, int y) const
{
    for (const auto& car : cars)
    {
        if (!car.IsBeingDriven() && car.GetX() == x && car.GetY() == y)
        {
            return true;
        }
    }
    return false;
}

Car* Island::GetCarAt(int x, int y)
{
    for (auto& car : cars)
    {
        if (!car.IsBeingDriven() && car.GetX() == x && car.GetY() == y)
        {
            return &car;
        }
    }
    return nullptr;
}

Car* Island::GetNearestCar(const Player& player)
{
    std::vector<Car*> nearbyCars;

    // Find all cars within distance 1
    for (auto& car : cars)
    {
        if (!car.IsBeingDriven() && player.IsAdjacentTo(car.GetX(), car.GetY()))
        {
            nearbyCars.push_back(&car);
        }
    }

    if (nearbyCars.empty())
        return nullptr;

    // Return a random car from nearby cars
    int randomIndex = rand() % nearbyCars.size();
    return nearbyCars[randomIndex];
}