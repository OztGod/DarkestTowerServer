#include "FighterAttack.h"
#include "Hero.h"

FighterAttack::FighterAttack()
	:Skill(SkillType::FIGHTER_ATTACK, 3, 0)
{
}

bool FighterAttack::isActEnable(Point pos) const
{
	if (pos.y == 0)
		return true;

	return false;
}

Range FighterAttack::getRange(Point pos) const
{
	Range res;

	res.isMyField = false;
	res.pos.push_back(Point(2 - pos.x, 0));
}

std::vector<Point> FighterAttack::getEffectRange(Point pos) const
{
	return { Point(0, 0) };
}

void FighterAttack::doSkill(Point pos, Hero* target, bool isAlly)
{
	//적군에게만 영향
	if (isAlly)
		return;

	if (!isHeroInEffect(pos, target, isAlly))
		return;

	target->damage(2);
}
