#include "GameEngine.h"
#include <iostream>
#include <string>

int main()
{
    GameEngine game;

    if (!game.Initialize("config.txt"))
    {
        std::cout << "Error: No config file found!!" << std::endl;
        return -1;
    }

    game.Run();

    return 0;
}