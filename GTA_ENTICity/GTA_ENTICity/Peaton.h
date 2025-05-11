#pragma once
#include "Map.h"

struct Peaton 
{
    int x, y;
    bool isDead;
    char movimiento; // H para horizontal o V para vertical
    int isla;

    Peaton(int posx, int posy, int islaIdx) 
    {
        x = posx;
        y = posy;
        isDead = false;
        isla = islaIdx;
        movimiento = (rand() % 2 == 0) ? 'H' : 'V';
    }

    // Verificar si CJ está cerca
    bool CJEstaCerca(const Carl& cj) 
    {
        return (abs(x - cj.x) <= 1 && abs(y - cj.y) <= 1);
    }

    // Mover peatón aleatoriamente evitando paredes
    void Mover(char** mapa, int anchoMapa, int altoMapa) 
    {
        if (isDead || CJEstaCerca(cjGlobal)) return; //Parar si el jugador esta cerca

        if (movimiento == 'H') 
        {
            int dx = (rand() % 2 == 0) ? 1 : -1; // direccion random 
            if (x + dx >= 0 && x + dx < anchoMapa && mapa[y][x + dx] != WALL) //comproba si la nueva posicion es valida
            {
                x += dx;
            }
        }
        else 
        {
            int dy = (rand() % 2 == 0) ? 1 : -1; // lo mismo verticalmente
            if (y + dy >= 0 && y + dy < altoMapa && mapa[y + dy][x] != WALL) 
            {
                y += dy;
            }
        }
    }
};