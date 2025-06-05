#pragma once
#include "Peaton.h"
#include "Car.h"
#include <vector>

struct Money
{
    int x;
    int y;
    int amount;

    Money(int posX, int posY, int amt) : x(posX), y(posY), amount(amt) {}
};

class Island
{

private:
    int islandId;
    std::vector<Peaton> peatones;
    std::vector<Car> cars;
    std::vector<Money> moneyDrops;
    int maxMoney;
    int numCars;

    void GenerateRandomPosition(int mapWidth, int mapHeight, int& x, int& y) const;
    void DropMoney(int x, int y, const Map& gameMap);

public:
    Island(int id, int numPeatones, int maxMoneyAmount);
    Island(int id, int numPeatones, int maxMoneyAmount, int numCarAmount);

    void GenerateInitialPeatones(int mapWidth, int mapHeight);
    void GenerateInitialCars(int mapWidth, int mapHeight);
    void UpdatePeatones(const Map& gameMap, const Player& player);
    void ProcessPlayerAttack(const Player& player, const Map& gameMap);
    void ProcessMoneyCollection(Player& player);
    void ProcessCarHitPeaton(const Player& player, const Map& gameMap);

    // Collision detection
    bool HasPeatonAt(int x, int y) const;
    bool HasMoneyAt(int x, int y) const;
    bool HasCarAt(int x, int y) const;
    Car* GetCarAt(int x, int y);
    Car* GetNearestCar(const Player& player);

    // Getters
    int GetIslandId() const { return islandId; }
    const std::vector<Car>& GetCars() const { return cars; }
};