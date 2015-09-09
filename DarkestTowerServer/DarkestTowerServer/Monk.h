#pragma once
#include "Hero.h"

class Monk : public Hero
{
public:
	Monk(int idx);
	~Monk() override = default;
};