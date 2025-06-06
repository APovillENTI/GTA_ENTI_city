#include "Map.h"
#include "Player.h"
#include "GameEngine.h"
#include <fstream>
#include <sstream>

Config::Config() : mapWidth(0), mapHeight(0), CJ_hp(0), CJ_pwr(0), dineroCruzarSF(0), dineroCruzarLV(0), numPeatonesLS(0), maxDineroLS(0), maxHealthLS(0),
pwrLS(0), numPeatonesSF(0), maxDineroSF(0), maxHealthSF(0), pwrSF(0), numPeatonesLV(0), maxDineroLV(0), maxHealthLV(0), pwrLV(0), numCochesLS(0), numCochesSF(0), 
numCochesLV(0) {}

bool Config::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    std::string line;

    // dimentions
    if (getline(file, line))
    {
        size_t sep = line.find(';');
        if (sep != std::string::npos)
        {
            mapWidth = std::stoi(line.substr(0, sep));
            mapHeight = std::stoi(line.substr(sep + 1));
        }
    }

    // CJ
    if (getline(file, line))
    {
        size_t sep = line.find(';');
        if (sep != std::string::npos)
        {
            CJ_hp = std::stoi(line.substr(0, sep));
            CJ_pwr = std::stoi(line.substr(sep + 1));
        }
    }

    // Peajes
    if (getline(file, line))
    {
        size_t sep = line.find(';');
        if (sep != std::string::npos)
        {
            dineroCruzarSF = std::stoi(line.substr(0, sep));
            dineroCruzarLV = std::stoi(line.substr(sep + 1));
        }
    }
    
    // Los Santos
    if (getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;

        if (getline(iss, token, ';')) numPeatonesLS = std::stoi(token);
        if (getline(iss, token, ';')) maxDineroLS = std::stoi(token);
        if (getline(iss, token, ';')) maxHealthLS = std::stoi(token);
        if (getline(iss, token, ';')) pwrLS = std::stoi(token);
    }

    //  San Fierro
    if (getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;

        if (getline(iss, token, ';')) numPeatonesSF = std::stoi(token);
        if (getline(iss, token, ';')) maxDineroSF = std::stoi(token);
        if (getline(iss, token, ';')) maxHealthSF = std::stoi(token);
        if (getline(iss, token, ';')) pwrSF = std::stoi(token);
    }

    //  las Venturas
    if (getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;

        if (getline(iss, token, ';')) numPeatonesLV = std::stoi(token);
        if (getline(iss, token, ';')) maxDineroLV = std::stoi(token);
        if (getline(iss, token, ';')) maxHealthLV = std::stoi(token);
        if (getline(iss, token, ';')) pwrLV = std::stoi(token);
    }

    // num cars
    if (getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;

        if (getline(iss, token, ';')) numCochesLS = std::stoi(token);
        if (getline(iss, token, ';')) numCochesSF = std::stoi(token);
        if (getline(iss, token, ';')) numCochesLV = std::stoi(token);
    }

    file.close();
    return true;
}

Map::Map() : cells(nullptr), width(0), height(0)
{
}

Map::~Map()
{
    Cleanup();
}

bool Map::Initialize(const Config& config)
{
    width = config.GetMapWidth();
    height = config.GetMapHeight();
    moneySF = config.GetDineroCruzarSF();
    moneyLS = config.GetDineroCruzarLV();

    // allocate memory
    cells = new Cell * [height];
    for (int i = 0; i < height; i++)
    {
        cells[i] = new Cell[width];

        for (int j = 0; j < width; j++)
        {
            // detect borders
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                cells[i][j] = Cell(CellType::WALL);
            }
            else
            {
                cells[i][j] = Cell(CellType::EMPTY);
            }
        }
    }

    CreateBridges();
    return true;
}

void Map::CreateBridges()
{
    int sep1 = width / 3;
    int sep2 = 2 * width / 3;

    for (int i = 1; i < height - 1; i++)
    {
        cells[i][sep1] = Cell(CellType::WALL);
        cells[i][sep2] = Cell(CellType::WALL);
    }

    int bridgeY = height / 2;
    cells[bridgeY][sep1] = Cell(CellType::BRIDGE);
    cells[bridgeY][sep2] = Cell(CellType::BRIDGE);
}

void Map::Cleanup()
{
    //dealloc map memory 
    if (cells != nullptr)
    {
        for (int i = 0; i < height; i++)
        {
            delete[] cells[i];
        }
        delete[] cells;
        cells = nullptr;
    }
}

bool Map::IsWalkable(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return false;
    }
    return cells[y][x].type != CellType::WALL;
}

bool Map::IsBridge(int x, int y) const
{
    return cells[y][x].type == CellType::BRIDGE;
}

void Map::EraseBridge(int x, int y) const
{
    cells[y][x].type = CellType::EMPTY;
}

char Map::GetCellDisplayChar(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return ' ';
    }

    switch (cells[y][x].type)
    {
        case CellType::WALL:
            return 'X';

        case CellType::BRIDGE:
            return 'T';

        case CellType::EMPTY:

        default:
            return ' ';
    }
}