#include "MonkAttack.h"
#include "Hero.h"
#include <algorithm>

MonkAttack::MonkAttack()
	:Skill(SkillType::MONK_ATTACK, 2, 0, false)
{
}

bool MonkAttack::isActEnable(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	if (pos.y == 2)
		return false;

	return true;
}

Range MonkAttack::getRange(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	Range res;

	res.isMyField = isMyField;
	Point front = Point(2 - pos.x, 0);
	Point middle = Point(2 - pos.x, 1);

	res.pos.push_back(front);

	for (int i = 0; i < enemy.size(); i++)
	{
		if (enemy[i]->getPos() == front)
			return res;
	}

	//front에 아무도 없으면 middle까지 가능
	res.pos.push_back(middle);

	return res;
}

std::vector<Point> MonkAttack::getEffectRange(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	return { Point(0,0) };
}

bool MonkAttack::doSkill(Point pos, Hero* user, Hero * target, const UHeroVec& ally, const UHeroVec& enemy) const
{
	target->damage(2);

	return true;
}
