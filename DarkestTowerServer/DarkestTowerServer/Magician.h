#pragma once
#include "Hero.h"

class Magician : public Hero
{
public:
	Magician() = default;
	Magician(int idx);
	~Magician() override = default;

private:
};