#include "Snipe.h"

Snipe::Snipe()
	:Skill(SkillType::ARCHER_SNIPE, 3, 1, false)
{
}

bool Snipe::isActEnable(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	if (pos.y == 2)
		true;

	return false;
}

Range Snipe::getRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	Range res;

	res.isMyField = isMyField;
	res.pos.push_back({ 0,1 });
	res.pos.push_back({ 1,1 });
	res.pos.push_back({ 2,1 });
	res.pos.push_back({ 0,2 });
	res.pos.push_back({ 1,2 });
	res.pos.push_back({ 2,2 });

	return res;
}

std::vector<Point> Snipe::getEffectRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	return{ Point(0,0) };
}

bool Snipe::doSkill(Point pos, Hero * user, Hero * target, const UHeroVec & ally, const UHeroVec & enemy) const
{
	damage(2, user, target);

	return true;
}
