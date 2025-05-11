#include "Map.h"
#include "Peaton.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

void LeerConfig(const std::string& archivo, Config& config) 
{
    std::ifstream f(archivo);
    std::string linea;

    // Leer dimensiones del mapa
    getline(f, linea);
    size_t sep = linea.find(';');
    config.anchoMapa = std::stoi(linea.substr(0, sep));
    config.altoMapa = std::stoi(linea.substr(sep + 1));

    // Leer datos de Los Santos
    getline(f, linea);
    sep = linea.find(';');
    config.numPeatonesLS = std::stoi(linea.substr(0, sep));
    linea = linea.substr(sep + 1);
    sep = linea.find(';');
    config.dineroCruzarSF = std::stoi(linea.substr(0, sep));
    config.maxDineroLS = std::stoi(linea.substr(sep + 1));

    // Leer datos de San Fierro
    getline(f, linea);
    sep = linea.find(';');;
    config.numPeatonesSF = std::stoi(linea.substr(0, sep));
    linea = linea.substr(sep + 1);
    sep = linea.find(';');
    config.dineroCruzarLV = std::stoi(linea.substr(0, sep));
    config.maxDineroSF = std::stoi(linea.substr(sep + 1));
}

void InicializarMapa(char**& mapa, const Config& config) 
{
    mapa = new char* [config.altoMapa]; // crear memoria prar las  columnas

    for (int i = 0; i < config.altoMapa; i++) 
    {
        mapa[i] = new char[config.anchoMapa];// alocar memoria para as filas

        for (int j = 0; j < config.anchoMapa; j++) 
        {
            if (i == 0 || i == config.altoMapa - 1 || j == 0 || j == config.anchoMapa - 1) // si se encuentra en el borde
            {
                mapa[i][j] = WALL;
            }
            else 
            {
                mapa[i][j] = EMPTY;
            }
        }
    }

    // Crear puentes entre islas
    int sep1 = config.anchoMapa / 3;
    int sep2 = 2 * config.anchoMapa / 3; //valor y (horizontal) de cada separacion

    for (int i = 1; i < config.altoMapa - 1; i++) 
    {
        mapa[i][sep1] = WALL;
        mapa[i][sep2] = WALL;
    }
    mapa[config.altoMapa / 2][sep1] = EMPTY;
    mapa[config.altoMapa / 2][sep2] = EMPTY;
}

void DibujarVista(char** mapa, const Carl& cj, const Config& config, const std::vector<Peaton>& peatones)
{
    system("cls");

    int vistaAncho = 21, vistaAlto = 21;

    int inicioX = cj.x - vistaAncho / 2;
    int inicioY = cj.y - vistaAlto / 2;

    for (int i = inicioY; i < inicioY + vistaAlto; i++) 
    {
        for (int j = inicioX; j < inicioX + vistaAncho; j++) 
        {
            if (i == cj.y && j == cj.x) //imprimir cj
            {
                std::cout << cj.direccion;
                continue;
            }
            if (i < 0 || j < 0 || i >= config.altoMapa || j >= config.anchoMapa) 
            {
                std::cout << EMPTY;
                continue;
            }

            // Dibujar dinero
            bool hayDinero = false;
            for (const Dinero& d : dineroGlobal) //para cada dinero en la lista global 
            {
                if (d.x == j && d.y == i) 
                {
                    std::cout << DINERO;
                    hayDinero = true;
                    break;
                }
            }
            if (hayDinero) continue;

            // Dibujar peatones
            bool hayPeaton = false;
            for (const Peaton& p : peatones) 
            {
                if (p.x == j && p.y == i && !p.isDead) 
                {
                    std::cout << PEATON;
                    hayPeaton = true;
                    break;
                }
            }
            if (!hayPeaton) std::cout << mapa[i][j];
        }
        std::cout << "\n";
    }
    std::cout << "Dinero: $" << cj.dinero << "\n";
}