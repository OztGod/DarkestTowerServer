#include "ArcherAttack.h"
#include "Hero.h"

ArcherAttack::ArcherAttack()
	:Skill(SkillType::ARCHER_ATTACK, 2, 0, false)
{
}

bool ArcherAttack::isActEnable(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	if (pos.y == 0)
		return false;

	return true;
}

Range ArcherAttack::getRange(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	Range res;

	res.isMyField = isMyField;
	res.pos.push_back(Point(2 - pos.x, 1));
	res.pos.push_back(Point(2 - pos.x, 2));

	return res;
}

std::vector<Point> ArcherAttack::getEffectRange(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	return { Point(0, 0) };
}

bool ArcherAttack::doSkill(Point pos, Hero* user, Hero * target, const UHeroVec& ally, const UHeroVec& enemy) const
{
	damage(2, user, target);

	return true;
}
