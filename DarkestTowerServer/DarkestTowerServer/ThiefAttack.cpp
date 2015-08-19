#include "ThiefAttack.h"
#include "Hero.h"

ThiefAttack::ThiefAttack()
	:Skill(SkillType::THIEF_ATTACK, 1, 0, false)
{
}

bool ThiefAttack::isActEnable(Point pos, const Map & map, int t) const
{
	if (pos.y == 2)
		return false;

	return true;
}

Range ThiefAttack::getRange(Point pos, const Map & map, int t) const
{
	Range res;

	res.isMyField = isMyField;
	Point front = Point(2 - pos.x, 0);
	Point middle = Point(2 - pos.x, 1);
	Point back = Point(2 - pos.x, 2);

	res.pos.push_back(front);

	//front에 아무도 없으면 middle까지 가능
	if (map.get(front.x, front.y, (t + 1) % 2))
		return res;

	res.pos.push_back(middle);

	if (map.get(middle.x, middle.y, (t + 1) % 2))
		return res;

	res.pos.push_back(back);

	return res;
}

std::vector<Point> ThiefAttack::getEffectRange(Point pos) const
{
	return { Point(0,0) };
}

bool ThiefAttack::doSkill(Point pos, Hero * user, Hero * target, bool isAlly) const
{
	//적군에게만 영향
	if (isAlly)
		return false;

	if (!isHeroInEffect(pos, target, isAlly))
		return false;

	target->damage(1);

	return true;
}
