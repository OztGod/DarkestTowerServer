#pragma once
#include "Skill.h"

class FighterAttack : public Skill
{
public:
	FighterAttack();
	~FighterAttack() override = default;

	bool isActEnable(Point pos) const override;

	Range getRange(Point pos) const override;

	std::vector<Point> getEffectRange(Point pos) const override;

	bool doSkill(Point pos, Hero* target, bool isAlly) const override;

private:
};