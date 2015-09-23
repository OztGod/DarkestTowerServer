#pragma once
#include "Hero.h"

class Monk : public Hero
{
public:
	Monk() = default;
	Monk(int idx);
	~Monk() override = default;
};