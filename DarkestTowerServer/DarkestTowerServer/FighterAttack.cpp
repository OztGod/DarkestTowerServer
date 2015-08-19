#include "FighterAttack.h"
#include "Hero.h"

FighterAttack::FighterAttack()
	:Skill(SkillType::FIGHTER_ATTACK, 3, 0, false)
{
}

bool FighterAttack::isActEnable(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	if (pos.y == 0)
		return true;

	return false;
}

Range FighterAttack::getRange(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	Range res;

	res.isMyField = isMyField;
	res.pos.push_back(Point(2 - pos.x, 0));

	return res;
}

std::vector<Point> FighterAttack::getEffectRange(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	return { Point(0, 0) };
}

bool FighterAttack::doSkill(Point pos, Hero* user, Hero* target, const UHeroVec& ally, const UHeroVec& enemy) const
{
	target->damage(2);

	return true;
}
