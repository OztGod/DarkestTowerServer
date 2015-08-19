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

	return res;
}

std::vector<Point> FighterAttack::getEffectRange(Point pos) const
{
	return { Point(0, 0) };
}

bool FighterAttack::doSkill(Point pos, Hero* target, bool isAlly) const
{
	//�������Ը� ����
	if (isAlly)
		return false;

	if (!isHeroInEffect(pos, target, isAlly))
		return false;

	target->damage(2);

	return true;
}
