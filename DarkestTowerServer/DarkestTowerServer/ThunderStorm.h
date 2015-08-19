#pragma once
#include "Skill.h"

class ThunderStorm : public Skill
{
public:
	ThunderStorm();
	~ThunderStorm() override = default;

	bool isActEnable(Point pos, const Map& map, int t) const override;

	Range getRange(Point pos, const Map& map, int t) const override;

	std::vector<Point> getEffectRange(Point pos) const override;

	bool doSkill(Point pos, Hero* user, Hero* target, bool isAlly) const override;

private:
};