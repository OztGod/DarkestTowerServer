#pragma once
#include "pcPacket.h"
#include "Skill.h"

struct Point
{
	int x, y;
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
