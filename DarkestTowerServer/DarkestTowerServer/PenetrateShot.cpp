#include "PenetrateShot.h"

PenetrateShot::PenetrateShot()
	:Skill(SkillType::ARCHER_PENETRATE_SHOT, 4, 4, false)
{
}

bool PenetrateShot::isActEnable(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	return true;
}

Range PenetrateShot::getRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	Range res;

	res.isMyField = isMyField;
	res.pos.push_back({ 2 - pos.x, 0 });
	res.pos.push_back({ 2 - pos.x, 1 });
	res.pos.push_back({ 2 - pos.x, 2 });

	return res;
}

std::vector<Point> PenetrateShot::getEffectRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	return
	{
		Point(pos.x,0 - pos.y),
		Point(pos.x,1 - pos.y),
		Point(pos.x,2 - pos.y)
	};
}

bool PenetrateShot::doSkill(Point pos, Hero * user, Hero * target, const UHeroVec & ally, const UHeroVec & enemy) const
{
	damage(2, user, target);

	return true;
}
