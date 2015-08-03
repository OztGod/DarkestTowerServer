#pragma once
#include "pcPacket.h"

struct Point
{
	int x, y;
};


struct Skill
{
	int idx;
	int level;
};

struct Hero
{
	HeroClass type;
	int hp;
	int maxHp;
	int act;
	int maxAct;
	Point pos;
	std::vector<Skill> skills;
};
