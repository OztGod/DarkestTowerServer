#include "PriestAttack.h"

PriestAttack::PriestAttack()
	:Skill(SkillType::PRIEST_ATTACK, 3, 0, false)
{
}

bool PriestAttack::isActEnable(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	if (pos.y == 0 || pos.y == 1)
		return true;

	return false;
}

Range PriestAttack::getRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	Range res;
	res.isMyField = isMyField;
	res.pos.push_back({ 2 - pos.x, 0 });
	
	return res;
}

std::vector<Point> PriestAttack::getEffectRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	return{ Point(0,0) };
}

bool PriestAttack::doSkill(Point pos, Hero * user, Hero * target, const UHeroVec & ally, const UHeroVec & enemy) const
{
	damage(1, user, target);

	return true;
}
