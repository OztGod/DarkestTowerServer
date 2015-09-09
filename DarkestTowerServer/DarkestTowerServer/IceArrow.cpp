#include "IceArrow.h"
#include "Hero.h"
#include "Ice.h"

IceArrow::IceArrow()
	:Skill(SkillType::MAGICIAN_ICE_ARROW, 3, 2, false)
{
}

bool IceArrow::isActEnable(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	if (pos.y == 1 || pos.y == 2)
		return true;

	return false;
}

Range IceArrow::getRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	Range res;
	res.isMyField = isMyField;
	res.pos.push_back({ 2 - pos.x,0 });
	res.pos.push_back({ 2 - pos.x,1 });

	return res;
}

std::vector<Point> IceArrow::getEffectRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	return{ Point(0,0) };
}

bool IceArrow::doSkill(Point pos, Hero * user, Hero * target, const UHeroVec & ally, const UHeroVec & enemy) const
{
	damage(3, user, target);

	target->addState(std::make_unique<Ice>(user->getIdx(), isMyField, target->getIdx(), 1, 1));

	return true;
}
