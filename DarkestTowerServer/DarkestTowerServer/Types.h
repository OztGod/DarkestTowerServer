#pragma once
#include <vector>
#include <memory>

class Hero;
using UHeroVec = std::vector<std::unique_ptr<Hero>>;

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