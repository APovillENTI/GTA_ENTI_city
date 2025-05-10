#include <Windows.h>
#include "Map.h"
#include "Peaton.h"

const char UP = '^';
const char DOWN = 'v';
const char LEFT = '<';
const char RIGHT = '>';

const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

#define KEY_UP VK_UP
#define KEY_DOWN VK_DOWN
#define KEY_LEFT VK_LEFT
#define KEY_RIGHT VK_RIGHT
#define KEY_ATTACK VK_SPACE
#define KEY_ESC VK_ESCAPE

bool KeyPressed(int key);

struct Carl {
    int x, y;
    char direction;
    int dinero;

    Carl() {
        x = 0;
        y = 0;
        direction = UP;
        dinero = 0;
    }

    void MoveCarl(int filasMapa, int columnasMapa, char** map) 
	{
        map[x][y] = EMPTY;
        if (KeyPressed(KEY_UP)) {
            if (map[x][y - 1] != WALL && map[x][y - 1] != PEATON)
                y--;
            direction = UP;
        }
        else if (KeyPressed(KEY_DOWN)) {
            if (map[x][y + 1] != WALL && map[x][y + 1] != PEATON)
                y++;
            direction = DOWN;
        }
        else if (KeyPressed(KEY_LEFT)) {
            if (map[x - 1][y] != WALL && map[x - 1][y] != PEATON)
                x--;
            direction = LEFT;
        }
        else if (KeyPressed(KEY_RIGHT)) {
            if (map[x + 1][y] != WALL && map[x + 1][y] != PEATON)
                x++;
            direction = RIGHT;
        }

        map[x][y] = direction;
    }
};


