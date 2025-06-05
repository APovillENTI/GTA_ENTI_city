#pragma once
#include "Map.h"
#include "Player.h"
#include "Island.h"
#include <vector>
#include <string>
#include <Windows.h>

class GameEngine
{
private:
    static const int NUM_FPS = 5;

    Map gameMap;
    Player player;
    std::vector<Island> islands;
    bool isRunning;

    bool KeyPressed(int key) const;
    void ProcessInput();
    void Update();
    void Render();
    char GetDisplayCharAt(int x, int y) const;

public:
    GameEngine();
    ~GameEngine();

    std::vector<Island> GetIslands() const { return islands; }
    bool Initialize(const std::string& configFile);
    void Run();
    void Shutdown();
};