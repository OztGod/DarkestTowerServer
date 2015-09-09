#pragma once
#include "Hero.h"

class Archer : public Hero
{
public:
	Archer(int idx);
	~Archer() override = default;
};