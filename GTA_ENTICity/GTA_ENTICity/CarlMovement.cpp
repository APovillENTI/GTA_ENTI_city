#include "CarlMovement.h"

bool KeyPressed(int key) {
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

void GameLoop(Carl carl, char** map, int mapRows, int mapCols)
{
    UINT32 frameStart = GetTickCount();

    carl.MoveCarl(mapRows, mapCols, map);
    PrintView(mapCols, mapRows, map, carl);

    UINT32 frameTime = GetTickCount() - frameStart;
    if (FRAME_DELAY > frameTime)
    {
        Sleep(FRAME_DELAY - frameTime);
    }
}