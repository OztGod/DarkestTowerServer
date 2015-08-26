#include "ThiefAttack.h"
#include "Hero.h"
#include <algorithm>

ThiefAttack::ThiefAttack()
	:Skill(SkillType::THIEF_ATTACK, 1, 0, false)
{
}

bool ThiefAttack::isActEnable(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	if (pos.y == 2)
		return false;

	return true;
}

Range ThiefAttack::getRange(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	Range res;

	res.isMyField = isMyField;
	Point front = Point(2 - pos.x, 0);
	Point middle = Point(2 - pos.x, 1);
	Point back = Point(2 - pos.x, 2);

	res.pos.push_back(front);
	
	for (int i = 0; i < enemy.size(); i++)
	{
		if (enemy[i]->getPos() == front)
			return res;
	}

	//front에 아무도 없으면 middle까지 가능
	res.pos.push_back(middle);

	for (int i = 0; i < enemy.size(); i++)
	{
		if (enemy[i]->getPos() == middle)
			return res;
	}

	//middle에 아무도 없으면 back까지 가능
	res.pos.push_back(back);

	return res;
}

std::vector<Point> ThiefAttack::getEffectRange(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	return { Point(0,0) };
}

bool ThiefAttack::doSkill(Point pos, Hero* user, Hero * target, const UHeroVec& ally, const UHeroVec& enemy) const
{
	target->damage(1);

	return true;
}
