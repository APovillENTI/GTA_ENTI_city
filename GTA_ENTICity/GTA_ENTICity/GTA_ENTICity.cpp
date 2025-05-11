#include "Map.h"
#include <vector>
#include <ctime>
#include "CarlMovement.h"
#include "Peaton.h"

// Variables globales
Carl cjGlobal;
std::vector<Peaton> peatonesGlobales;
std::vector<Dinero> dineroGlobal;


int main() {
    srand(time(nullptr));
    Config config;
    LeerConfig("config.txt", config);

    // Inicializar mapa y CJ
    char** mapa;
    InicializarMapa(mapa, config);

    cjGlobal = { config.anchoMapa / 6, config.altoMapa / 2, 'v', 0 };

    // Generar peatones iniciales en Los Santos (isla 0)
    for (int i = 0; i < config.numPeatonesLS; i++) 
    {
        int x = rand() % (config.anchoMapa / 3) + 1;
        int y = rand() % (config.altoMapa - 2) + 1;
        peatonesGlobales.emplace_back(x, y, 0);
    }

    // Bucle principal
    while (true) 
    {
        if (KeyPressed(VK_ESCAPE)) break;

        // Movimiento de CJ
        MoverCarl(cjGlobal, mapa, config);

        // Mover peatones
        for (Peaton& p : peatonesGlobales) 
        {
            p.Mover(mapa, config.anchoMapa, config.altoMapa);
        }

        // Ataque con SPACE
        if (KeyPressed(VK_SPACE)) 
        {
            for (Peaton& p : peatonesGlobales)  // comprobamos cada peaton
            {
                if (!p.isDead && abs(p.x - cjGlobal.x) + abs(p.y - cjGlobal.y) == 1) //abs() para precision
                {
                    p.isDead = true;

                    dineroGlobal.push_back({ p.x, p.y, rand() % config.maxDineroLS + 1 });

                    // Regenerar peatón en la misma isla
                    int newX = rand() % (config.anchoMapa / 3) + 1;
                    int newY = rand() % (config.altoMapa - 2) + 1;
                    peatonesGlobales.emplace_back(newX, newY, 0);
                }
            }
        }

        // Recolectar dinero
        for (auto dollar = dineroGlobal.begin(); dollar != dineroGlobal.end();) //Recorer la lista de dinero global
        {
            if (dollar->x == cjGlobal.x && dollar->y == cjGlobal.y) //si la posicion del dollar i jugador coinicde
            {
                cjGlobal.dinero += dollar->cantidad; //sumar cantidad
                dollar = dineroGlobal.erase(dollar); //quitar de la lista gobal
            }
            else 
            {
                dollar++;
            }
        }

        // Dibujar vista
        DibujarVista(mapa, cjGlobal, config, peatonesGlobales);
        Sleep(100);
    }

    // Liberar memoria
    for (int i = 0; i < config.altoMapa; i++) delete[] mapa[i];
    delete[] mapa;
    return 0;
}