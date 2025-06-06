#include "GameEngine.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>

GameEngine::GameEngine() : isRunning(false), actualScene(Scene::MAIN_MENU)
{
}

GameEngine::~GameEngine()
{
    Shutdown();
}

void GameEngine::MainMenu()
{
    system("cls");
    std::cout << "GTA ENTI CITY" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    if (cursorOnPlay)
        std::cout << "PLAY <  EXIT GAME " << std::endl;
    else
        std::cout << "PLAY    EXIT GAME <" << std::endl;
}

void GameEngine::EndGame()
{
    system("cls");
    std::cout << "GAME OVER!" << std::endl;
    std::cout << std::endl;
    std::cout << "(PRESS SPACE)" << std::endl;
}

bool GameEngine::Initialize(const std::string& configFile)
{
    Config config;
    if (!config.LoadFromFile(configFile))
    {
        return false;
    }

    if (!gameMap.Initialize(config))
    {
        return false;
    }

    // Initialize player at starting position
    player.SetPosition(2, gameMap.GetHeight() / 2);
    player.SetHealth(config.GetCjHp());
    player.SetPower(config.GetCjPwr());

    // Initialize islands
    islands.clear();
    islands.emplace_back(0, config.GetNumPeatonesLS(), config.GetMaxDineroLS(), config.GetMaxHealthLS(), config.GetPwrLS(), config.GetNumCochesLS());
    islands.emplace_back(1, config.GetNumPeatonesSF(), config.GetMaxDineroSF(), config.GetMaxHealthSF(), config.GetPwrSF(), config.GetNumCochesSF());
    islands.emplace_back(2, config.GetNumPeatonesLV(), config.GetMaxDineroLV(), config.GetMaxHealthLV(), config.GetPwrLV(), config.GetNumCochesLV()); // Las Venturas - change later

    // Generate initial peatones and cars for each island
    for (Island& island : islands)
    {
        island.GenerateInitialPeatones(gameMap.GetWidth(), gameMap.GetHeight());
        island.GenerateInitialCars(gameMap.GetWidth(), gameMap.GetHeight());
    }

    isRunning = true;
    return true;
}

void GameEngine::Run()
{
    while (isRunning)
    {
        switch (actualScene)
        {
        case (Scene::MAIN_MENU):
            ProcessInput();
            Sleep(1000 / NUM_FPS);
            break;
        case (Scene::PLAY_SCENE):
            ProcessInput();
            Update();
            Render();
            Sleep(1000 / NUM_FPS);
            break;
        case (Scene::END_GAME):
            EndGame();
            ProcessInput();
            Sleep(1000 / NUM_FPS);
            break;
        }
    }
}

void GameEngine::Shutdown()
{
    isRunning = false;
}

bool GameEngine::KeyPressed(int key) const
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

void GameEngine::ProcessInput()
{
    if (KeyPressed(VK_ESCAPE))
    {
        isRunning = false;
        return;
    }

    if (actualScene == Scene::MAIN_MENU)
    {
        if (KeyPressed(VK_LEFT) || KeyPressed('A'))
        {
            cursorOnPlay = true;
            MainMenu();
        }
        if (KeyPressed(VK_RIGHT) || KeyPressed('D'))
        {
            cursorOnPlay = false;
            MainMenu();
        }
        if (KeyPressed(VK_SPACE))
        {
            if (cursorOnPlay)
                actualScene = Scene::PLAY_SCENE;
            else
                isRunning = false;
        }
        return;
    }

    if (actualScene == Scene::END_GAME)
    {
        if (KeyPressed(VK_SPACE))
        {
            actualScene = Scene::MAIN_MENU;
            Initialize("config.txt");
            MainMenu();
        }
        return;
    }

    // Car enter/exit
    if (KeyPressed('E'))
    {
        if (!player.IsDriving())
        {
            // Try to enter a nearby car
            for (Island& island : islands)
            {
                Car* nearestCar = island.GetNearestCar(player);
                if (nearestCar != nullptr)
                {
                    player.EnterCar(nearestCar);
                    break;
                }
            }
        }
        else
        {
            player.ExitCar(gameMap);
        }
        Sleep(100); // Prevent rapid key presses
    }
    
    // Movement, WASD not asked for but implemented for ease of mind
    if (KeyPressed('W') || KeyPressed(VK_UP))
    {
        player.Move(0, -1, gameMap);
        player.SetDirection(Direction::UP);
    }
    else if (KeyPressed('S') || KeyPressed(VK_DOWN))
    {
        player.Move(0, 1, gameMap);
        player.SetDirection(Direction::DOWN);
    }
    else if (KeyPressed('A') || KeyPressed(VK_LEFT))
    {
        player.Move(-1, 0, gameMap);
        player.SetDirection(Direction::LEFT);
    }
    else if (KeyPressed('D') || KeyPressed(VK_RIGHT))
    {
        player.Move(1, 0, gameMap);
        player.SetDirection(Direction::RIGHT);
    }
    else if (KeyPressed('J')) // Para probar que funciona el endgame, hay que quitarlo
        player.SetHealth(player.GetHealth() - 10); // Le quita 10 de vida al player
    if (KeyPressed(VK_SPACE) && !player.IsDriving())
    {
        for (Island& island : islands)
        {
            island.ProcessPlayerAttack(player, gameMap);
        }
    }
}

void GameEngine::Update()
{
    // Update islandsa
    if (player.GetHealth() > 0)
    {
        for (Island& island : islands)
        {
            island.UpdatePeatones(gameMap, player);
            island.ProcessMoneyCollection(player);

            if (player.IsDriving())
            {
                island.ProcessCarHitPeaton(player, gameMap);
            }
        }
    }
    else
    {
        actualScene = Scene::END_GAME;
    }
}

void GameEngine::Render()
{
    system("cls");

    // Render map
    for (int y = 0; y < gameMap.GetHeight(); y++)
    {
        for (int x = 0; x < gameMap.GetWidth(); x++)
        {
            char displayChar = GetDisplayCharAt(x, y);
            std::cout << displayChar;
        }
        std::cout << std::endl;
    }

    // Display player stats
    std::cout << "Money: $" << player.GetMoney() << std::endl;
    std::cout << "HP: " << player.GetHealth() << std::endl;
    // std::cout << "Position: (" << player.GetX() << ", " << player.GetY() << ")" << std::endl; // DEBUG

    if (player.IsDriving())
    {
        std::cout << "Status: Driving car" << std::endl;
        std::cout << "Controls: WASD/Arrow Keys to drive, E to exit car, ESC to quit" << std::endl;
    }
    else
    {
        std::cout << "Status: WALKING" << std::endl;
        std::cout << "Controls: WASD/Arrow Keys to move, SPACE to attack, E to enter car, ESC to quit" << std::endl;
    }
}

char GameEngine::GetDisplayCharAt(int x, int y) const
{
    // Check if player is at this position and not driving
    if (player.GetX() == x && player.GetY() == y && !player.IsDriving())
    {
        return player.GetDirectionChar();
    }

    // Check for cars (including driven cars)
    for (const Island& island : islands)
    {
        if (island.HasCarAt(x, y))
        {
            return 'C';
        }

        // Check if there's a car being driven at this position
        for (const Car& car : island.GetCars())
        {
            if (car.IsBeingDriven() && car.GetX() == x && car.GetY() == y)
            {
                return 'C';
            }
        }
    }

    // Check for peatones
    for (const Island& island : islands)
    {
        if (island.HasPeatonAt(x, y))
        {
            return 'P';
        }
    }

    // Check for money
    for (const Island& island : islands)
    {
        if (island.HasMoneyAt(x, y))
        {
            return '$';
        }
    }

    // Return map cell character
    return gameMap.GetCellDisplayChar(x, y);
}