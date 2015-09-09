#pragma once
#include "Hero.h"

class Thief : public Hero
{
public:
	Thief(int idx);
	~Thief() override = default;
};