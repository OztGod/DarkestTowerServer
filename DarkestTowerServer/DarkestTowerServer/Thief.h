#pragma once
#include "Hero.h"

class Thief : public Hero
{
public:
	Thief() = default;
	Thief(int idx);
	~Thief() override = default;
};