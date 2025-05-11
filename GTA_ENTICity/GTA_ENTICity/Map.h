#pragma once
#include <vector>
#include <string>

const char WALL = 'X';
const char EMPTY = ' ';
const char PEATON = 'P';
const char DINERO = '$';

struct Config;
struct Carl;
struct Peaton;
struct Dinero;

//guardar los datos de configuracion del mapa aquí
struct Config 
{
    int anchoMapa, altoMapa;
    int numPeatonesLS, dineroCruzarSF, maxDineroLS;
    int numPeatonesSF, dineroCruzarLV, maxDineroSF;
};

struct Carl 
{
    int x, y;
    char direccion;
    int dinero;

    Carl() : x(0), y(0), direccion('v'), dinero(0) {} // predeterminar valores a 0 pq sino da error :/

    Carl(int X, int Y, char dir, int Din)
    {
        x = X;
        y = Y;
        direccion = dir;
        dinero = Din;
    }
};

struct Dinero 
{
    int x, y;
    int cantidad;

    Dinero(int posX, int posY, int cant)
    {
        x = posX;
        y = posY;
        cantidad = cant;
    }
};

// Variables globales
extern Carl cjGlobal;
extern std::vector<Peaton> peatonesGlobales;
extern std::vector<Dinero> dineroGlobal;

// Funciones
void LeerConfig(const std::string& archivo, Config& config);
void InicializarMapa(char**& mapa, const Config& config);
void DibujarVista(char** mapa, const Carl& cj, const Config& config, const std::vector<Peaton>& peatones);