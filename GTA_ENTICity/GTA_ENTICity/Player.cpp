#include "Player.h"
#include "Car.h"
#include <cmath>
#include <cstdlib>

Player::Player() : x(0), y(0), direction(Direction::DOWN), money(0), currentCar(nullptr), isDriving(false){}

Player::Player(int startX, int startY, Direction startDir) : x(startX), y(startY), direction(startDir), money(0), currentCar(nullptr), isDriving(false){}

char Player::GetDirectionChar() const
{
    switch (direction)
    {
        case Direction::UP:
            return '^';

        case Direction::DOWN:
            return 'v';

        case Direction::LEFT:
            return '<';

        case Direction::RIGHT:
            return '>';

        default:
            return 'v';
    }
}

void Player::SetPosition(int newX, int newY)
{
    x = newX;
    y = newY;
}

void Player::SetDirection(Direction newDir)
{
    direction = newDir;
}

void Player::AddMoney(int amount)
{
    money += amount;
}

bool Player::Move(int dx, int dy, const Map& gameMap)
{
    //if driving
    if (isDriving && currentCar != nullptr)
    {
        return currentCar->Move(dx, dy, gameMap);
    }
    else
    {
        int newX = x + dx;
        int newY = y + dy;

        if (gameMap.IsWalkable(newX, newY))
        {
            x = newX;
            y = newY;
            return true;
        }
    }

    return false;
}

//true if dist = 1
bool Player::IsAdjacentTo(int targetX, int targetY) const
{
    int distanceX = abs(x - targetX);
    int distanceY = abs(y - targetY);

    return (distanceX + distanceY) == 1;
}

void Player::EnterCar(Car* car)
{
    if (car != nullptr && !isDriving)
    {
        currentCar = car;
        isDriving = true;
        currentCar->SetBeingDriven(true);

        x = car->GetX();
        y = car->GetY();
    }
}

void Player::ExitCar(const Map& gameMap)
{
    if (isDriving && currentCar != nullptr)
    {
        int carX = currentCar->GetX();
        int carY = currentCar->GetY();

        //All directions
        int dx[] = { -1, 1, 0, 0 };
        int dy[] = { 0, 0, -1, 1 };

        for (int i = 0; i < 4; i++)
        {
            int newX = carX + dx[i];
            int newY = carY + dy[i];

            if (gameMap.IsWalkable(newX, newY))
            {
                x = newX;
                y = newY;
                break;
            }
        }

        currentCar->SetBeingDriven(false);
        currentCar = nullptr;
        isDriving = false;
    }
}