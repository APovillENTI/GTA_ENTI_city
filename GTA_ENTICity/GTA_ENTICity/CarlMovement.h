#include <Windows.h>
#include "Map.h"

#define KEY_UP VK_UP
#define KEY_DOWN VK_DOWN
#define KEY_LEFT VK_LEFT
#define KEY_RIGHT VK_RIGHT
#define KEY_ATTACK VK_SPACE

bool KeyPressed(int key) 
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

void MoverCarl(Carl& cj, char** mapa, const Config& config) 
{
    int dx = 0;
    int dy = 0;
    char dir = cj.direccion;

    if (KeyPressed(KEY_UP)) 
    { 
        dy = -1; dir = '^'; 
    }
    else if (KeyPressed(KEY_DOWN)) 
    { 
        dy = 1; dir = 'v'; 
    }
    else if (KeyPressed(KEY_LEFT)) 
    {
        dx = -1; dir = '<'; 
    }
    else if (KeyPressed(KEY_RIGHT)) 
    { 
        dx = 1; dir = '>'; 
    }

    int newX = cj.x + dx;
    int newY = cj.y + dy;

    // Comprobar colision con paredes
    if (newX >= 0 && newX < config.anchoMapa && newY >= 0 && newY < config.altoMapa) 
    {
        if (mapa[newY][newX] != WALL) 
        {
            cj.x = newX;
            cj.y = newY;
            cj.direccion = dir;
        }
    }
}