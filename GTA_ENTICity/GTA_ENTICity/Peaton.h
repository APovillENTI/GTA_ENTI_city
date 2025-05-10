const char WALL = '=';
const char PEATON = 'P';

struct Peaton
{
	int x;
	int y;
	bool isDead;
	bool stop;

	Peaton(int posx, int posy)
	{
		isDead = false;
		stop = false;
		x = posx;
		y = posy;
	}

	bool CheckIfPlayer(char** map, char playerChar)
	{
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; i < 2; i++)
			{
				if (map[x + i][y + j] == playerChar)
					return true;
			}
		}
		return false;
	}

	void MovePeaton(int dir, char** map)
	{
		// 0 abajo, 1, arriba, 2 derecha, 3 izquierda
		bool moved = false;
		while (!moved)
		{
			switch (dir)
			{
				case 0:
					if (map[x][y - 1] == WALL)
						dir++;
					else
					{
						y--;
						moved = true;
					}
					break;
				case 1:
					if (map[x][y + 1] == WALL)
						dir++;
					else
					{
						y++;
						moved = true;
					}
					break;
				case 2:
					if (map[x + 1][y] == WALL)
						dir++;
					else
					{
						x++;
						moved = true;
					}
					break;
				case 3:
					if (map[x - 1][y] == WALL)
						dir++;
					else
					{
						x--;
						moved = true;
					}
					break;
			}
		}
	}
};
