#pragma once
#include "Map.h"
#include "Player.h"
#include "Island.h"
#include <vector>
#include <string>
#include <Windows.h>

enum class Scene {
    MAIN_MENU,
    PLAY_SCENE,
    END_GAME,
    COUNT
};

class GameEngine
{
private:
    static const int NUM_FPS = 5;

    Map gameMap;
    Player player;
    Scene actualScene;
    std::vector<Island> islands;
    bool isRunning;
    bool cursorOnPlay;

    bool KeyPressed(int key) const;
    void ProcessInput();
    void Update();
    void Render();
    char GetDisplayCharAt(int x, int y) const;

public:
    GameEngine();
    ~GameEngine();

    std::vector<Island> GetIslands() const { return islands; }
    void MainMenu();
    void EndGame();
    bool Initialize(const std::string& configFile);
    void Run();
    void Shutdown();
};