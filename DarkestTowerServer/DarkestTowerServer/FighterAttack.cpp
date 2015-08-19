#include "FighterAttack.h"
#include "Hero.h"

FighterAttack::FighterAttack()
	:Skill(SkillType::FIGHTER_ATTACK, 3, 0, false)
{
}

bool FighterAttack::isActEnable(Point pos, const Map& map, int t) const
{
	if (pos.y == 0)
		return true;

	return false;
}

Range FighterAttack::getRange(Point pos, const Map& map, int t) const
{
	Range res;

	res.isMyField = isMyField;
	res.pos.push_back(Point(2 - pos.x, 0));

	return res;
}

std::vector<Point> FighterAttack::getEffectRange(Point pos) const
{
	return { Point(0, 0) };
}

bool FighterAttack::doSkill(Point pos, Hero* user, Hero* target, bool isAlly) const
{
	//적군에게만 영향
	if (isAlly)
		return false;

	if (!isHeroInEffect(pos, target, isAlly))
		return false;

	target->damage(2);

	return true;
}
