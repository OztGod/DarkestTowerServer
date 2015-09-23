#pragma once
#include "Hero.h"

class Archer : public Hero
{
public:
	Archer() = default;
	Archer(int idx);
	~Archer() override = default;
};