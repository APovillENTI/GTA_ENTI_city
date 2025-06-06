#include "GameEngine.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>

GameEngine::GameEngine() : isRunning(false), actualScene(Scene::MAIN_MENU), bigSmoke(nullptr)
{
}

GameEngine::~GameEngine()
{
    Shutdown();
    if (bigSmoke != nullptr)
    {
        delete bigSmoke;
        bigSmoke = nullptr;
    }
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

    if (bigSmoke != nullptr)
    {
        delete bigSmoke;
    }

    int islandWidth = gameMap.GetWidth() / 3;
    int startX = 2 * islandWidth + 1; // Third island
    int endX = startX + islandWidth - 2;
    int smokeX = startX + (rand() % (endX - startX + 1));
    int smokeY = 1 + (rand() % (gameMap.GetHeight() - 2));

    bigSmoke = new BigSmoke(smokeX, smokeY, 300, 25);//helth and power


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
    if (KeyPressed(VK_SPACE) && !player.IsDriving())
    {
        for (Island& island : islands)
        {
            island.ProcessPlayerAttack(player, gameMap);
        }

        if (bigSmoke != nullptr && !bigSmoke->IsDead() && player.IsAdjacentTo(bigSmoke->GetX(), bigSmoke->GetY()))
        {
            bigSmoke->GetDamage(player.GetPower());
            if (bigSmoke->GetHP() <= 0)
            {
                bigSmoke->Kill();
                actualScene = Scene::END_GAME;
            }
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

            island.ProcessPeatonAttacks(player);

            if (player.IsDriving())
            {
                island.ProcessCarHitPeaton(player, gameMap);
            }
        }

        if (bigSmoke != nullptr && !bigSmoke->IsDead())
        {
            bigSmoke->Update(gameMap, player);

            if (bigSmoke->ShouldAttackPlayer(player) && !player.IsDriving())
            {
                int currentHealth = player.GetHealth();
                player.SetHealth(currentHealth - bigSmoke->GetPower());
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
    if (player.GetX() == x && player.GetY() == y && !player.IsDriving())
    {
        return player.GetDirectionChar();
    }

    if (bigSmoke != nullptr && !bigSmoke->IsDead() && bigSmoke->GetX() == x && bigSmoke->GetY() == y)
    {
        return 'B';
    }

    // Check for cars 
    for (const Island& island : islands)
    {
        if (island.HasCarAt(x, y))
        {
            return 'C';
        }

        // Check for driven cars
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