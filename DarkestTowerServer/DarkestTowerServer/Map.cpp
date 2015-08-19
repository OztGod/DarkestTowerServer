#include "Map.h"

Map::Map()
{
	reset();
}

void Map::reset()
{
	for (int t = 0; t < 2; t++)
	{
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				data[t][x][y] = false;
			}
		}
	}
}

void Map::move(Point prev, Point next, int t)
{
	std::swap(data[t][prev.x][prev.y], data[t][next.x][next.y]);
}
