#pragma once
#include <string> 

enum class CellType
{
    EMPTY,
    WALL,
    BRIDGE
};

struct Cell
{
    CellType type;

    Cell() : type(CellType::EMPTY) {}
    Cell(CellType t) : type(t) {}
};

class Config
{
private:
    int mapWidth;
    int mapHeight;
    int numPeatonesLS;
    int dineroCruzarSF;
    int maxDineroLS;
    int numPeatonesSF;
    int dineroCruzarLV;
    int maxDineroSF;
    int numCochesLS;
    int numCochesSF;
    int numCochesLV;

public:
    Config();
    bool LoadFromFile(const std::string& filename);

    // Getters
    int GetMapWidth() const { return mapWidth; }
    int GetMapHeight() const { return mapHeight; }
    int GetNumPeatonesLS() const { return numPeatonesLS; }
    int GetDineroCruzarSF() const { return dineroCruzarSF; }
    int GetMaxDineroLS() const { return maxDineroLS; }
    int GetNumPeatonesSF() const { return numPeatonesSF; }
    int GetDineroCruzarLV() const { return dineroCruzarLV; }
    int GetMaxDineroSF() const { return maxDineroSF; }
    int GetNumCochesLS() const { return numCochesLS; }
    int GetNumCochesSF() const { return numCochesSF; }
    int GetNumCochesLV() const { return numCochesLV; }
};

class Map
{
private:
    Cell** cells;
    int width;
    int height;

    void CreateBridges();

public:
    Map();
    ~Map();

    bool Initialize(const Config& config);
    void Cleanup();

    bool IsWalkable(int x, int y) const;
    char GetCellDisplayChar(int x, int y) const;

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
};