#include "FireBlast.h"
#include "Hero.h"
#include "Burn.h"

FireBlast::FireBlast()
	:Skill(SkillType::MAGICIAN_FIRE_BLAST, 3, 2, false)
{
}

bool FireBlast::isActEnable(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	if (pos.y == 1 || pos.y == 2)
		return true;

	return false;
}

Range FireBlast::getRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	Range res;

	res.isMyField = isMyField;

	res.pos.push_back({ 2 - pos.x,0 });
	res.pos.push_back({ 2 - pos.x,1 });

	return res;
}

std::vector<Point> FireBlast::getEffectRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	return { Point(0,0) };
}

bool FireBlast::doSkill(Point pos, Hero * user, Hero * target, const UHeroVec & ally, const UHeroVec & enemy) const
{
	damage(2, user, target);

	target->addState(std::make_unique<Burn>(user->getIdx(), isMyField, target->getIdx(), 1, 3));

	return true;
}
