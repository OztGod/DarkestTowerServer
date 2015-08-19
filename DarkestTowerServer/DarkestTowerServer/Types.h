#pragma once
#include <vector>

struct Point
{
	Point() : x(0), y(0) {}
	Point(int x_, int y_) : x(x_), y(y_) {}

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