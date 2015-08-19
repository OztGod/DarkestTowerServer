#pragma once
#include <vector>

struct Point
{
	int x, y;

	bool operator ==(const Point& rhs)
	{
		return x == rhs.x && y == rhs.y;
	}
};

struct Range
{
	std::vector<Point> pos;
	bool isMyField;
};