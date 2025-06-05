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

    int CJ_hp;
    int CJ_pwr;

    int dineroCruzarSF;
    int dineroCruzarLV;

    int numPeatonesLS;
    int maxDineroLS;
    int maxHealthLS;
    int pwrLS;

    int numPeatonesSF;
    int maxDineroSF;
    int maxHealthSF;
    int pwrSF;

    int numPeatonesLV;
    int maxDineroLV;
    int maxHealthLV;
    int pwrLV;

    int numCochesLS;
    int numCochesSF;
    int numCochesLV;

public:
    Config();
    bool LoadFromFile(const std::string& filename);

    // Getters
    int GetMapWidth() const { return mapWidth; }
    int GetMapHeight() const { return mapHeight; }

    int GetCjHp() const { return CJ_hp; }
    int GetCjPwr() const { return CJ_pwr; }

    int GetDineroCruzarSF() const { return dineroCruzarSF; }
    int GetDineroCruzarLV() const { return dineroCruzarLV; }

    int GetNumPeatonesLS() const { return numPeatonesLS; }
    int GetMaxDineroLS() const { return maxDineroLS; }
    int GetMaxHealthLS() const { return maxHealthLS; }
    int GetPwrLS() const { return pwrLS; }

    int GetNumPeatonesSF() const { return numPeatonesSF; }
    int GetMaxDineroSF() const { return maxDineroSF; }
    int GetMaxHealthSF() const { return maxHealthSF; }
    int GetPwrSF() const { return pwrSF; }

    int GetNumPeatonesLV() const { return numPeatonesLV; }
    int GetMaxDineroLV() const { return maxDineroLV; }
    int GetMaxHealthLV() const { return maxHealthLV; }
    int GetPwrLV() const { return pwrLV; }

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
    int moneyLS;
    int moneySF;

    void CreateBridges();

public:
    Map();
    ~Map();

    bool Initialize(const Config& config);
    void Cleanup();
    void EraseBridge(int x, int y) const;

    bool IsWalkable(int x, int y) const;
    bool IsBridge(int x, int y) const;
    char GetCellDisplayChar(int x, int y) const;
    
    int GetMoneyLS() const { return moneyLS; }
    int GetMoneySF() const { return moneySF; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
};