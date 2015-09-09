#pragma once
#include "Skill.h"

class Poison : public Skill
{
public:
	Poison();
	~Poison() override = default;

	bool isActEnable(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const override;

	Range getRange(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const override;

	std::vector<Point> getEffectRange(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const override;

	bool doSkill(Point pos, Hero* user, Hero* target, const UHeroVec& ally, const UHeroVec& enemy) const override;

private:
};