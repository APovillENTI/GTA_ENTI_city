#include <iostream>
#include "Map.h"
#include "CarlMovement.h"

void PrintView(int width, int height, char** map, Carl carl)
{
	int LimitesH = width / 2;
	int LimitesV = height / 2;

	for (int i = carl.y - LimitesV; i < carl.y + LimitesV; i++)
	{
		for (int j = carl.x - LimitesH; j < carl.x + LimitesH; j++)
		{
			if (j < 0 || i < 0 || j > width || i > height)
				std::cout << EMPTY;
			else
				std::cout << map[i][j];
		}
		std::cout << std::endl;
	}
}
