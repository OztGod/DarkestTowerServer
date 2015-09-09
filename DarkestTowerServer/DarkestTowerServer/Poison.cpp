#include "Poison.h"
#include "Hero.h"
#include "PoisonState.h"

Poison::Poison()
	:Skill(SkillType::THIEF_POISON, 2, 2, false)
{
}

bool Poison::isActEnable(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	if (pos.y == 0 || pos.y == 1)
		return true;

	return false;
}

Range Poison::getRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	Range res;

	res.isMyField = isMyField;
	res.pos.push_back({ 2 - pos.x,0 });
	res.pos.push_back({ 2 - pos.x,1 });
	
	return res;
}

std::vector<Point> Poison::getEffectRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	return{ Point(0,0) };
}

bool Poison::doSkill(Point pos, Hero * user, Hero * target, const UHeroVec & ally, const UHeroVec & enemy) const
{
	damage(2, user, target);

	target->addState(std::make_unique<PoisonState>(user->getIdx(), isMyField, target->getIdx(), 1, 3));
	return true;
}
