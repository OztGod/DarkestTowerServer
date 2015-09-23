#pragma once
#include "Hero.h"

class Priest : public Hero
{
public:
	Priest() = default;
	Priest(int idx);
	~Priest() override = default;
};