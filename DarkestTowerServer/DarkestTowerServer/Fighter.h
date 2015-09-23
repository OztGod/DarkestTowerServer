#pragma once
#include "Hero.h"

class Fighter : public Hero
{
public:
	Fighter() = default;
	Fighter(int idx);
	~Fighter() override = default;

private:
};