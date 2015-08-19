#include "MonkAttack.h"
#include "Hero.h"

MonkAttack::MonkAttack()
	:Skill(SkillType::MONK_ATTACK, 2, 0, false)
{
}

bool MonkAttack::isActEnable(Point pos, const Map& map, int t) const
{
	if (pos.y == 2)
		return false;

	return true;
}

Range MonkAttack::getRange(Point pos, const Map& map, int t) const
{
	Range res;

	res.isMyField = isMyField;
	Point front = Point(2 - pos.x, 0);
	Point middle = Point(2 - pos.x, 1);

	res.pos.push_back(front);

	//front에 아무도 없으면 middle까지 가능
	if (!map.get(front.x, front.y, (t + 1) % 2))
	{
		res.pos.push_back(middle);
	}

	return res;
}

std::vector<Point> MonkAttack::getEffectRange(Point pos) const
{
	return { Point(0,0) };
}

bool MonkAttack::doSkill(Point pos, Hero* user, Hero * target, bool isAlly) const
{
	//적군에게만 영향
	if (isAlly)
		return false;

	if (!isHeroInEffect(pos, target, isAlly))
		return false;

	target->damage(2);

	return true;
}
