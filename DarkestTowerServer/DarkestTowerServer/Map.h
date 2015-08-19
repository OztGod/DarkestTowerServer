#pragma once
#include "Types.h"

class Map
{
public:
	Map();
	~Map() = default;

	void reset();

	void move(Point prev, Point next, int t);

	void set(int x, int y, int t, bool value)
	{
		data[t][x][y] = value;
	}

	bool get(int x, int y, int t) const
	{
		return data[t][x][y];
	}

private:
	bool data[2][3][3];
};