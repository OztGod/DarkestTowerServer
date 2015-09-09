#pragma once
#include "Hero.h"

class Priest : public Hero
{
public:
	Priest(int idx);
	~Priest() override = default;
};