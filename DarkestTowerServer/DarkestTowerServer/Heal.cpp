#include "Heal.h"
#include "Hero.h"

Heal::Heal()
	:Skill(SkillType::PRIEST_HEAL, 3, 1, true)
{
}

bool Heal::isActEnable(Point pos, const Map & map, int t) const
{
	//힐은 무조건 가능
	return true;
}

Range Heal::getRange(Point pos, const Map & map, int t) const
{
	Range res;

	res.isMyField = isMyField;

	//주변 4방향
	std::vector<Point> cand =
	{
		Point(pos.x - 1,pos.y),
		Point(pos.x + 1,pos.y),
		Point(pos.x,pos.y - 1),
		Point(pos.x,pos.y + 1)
	};

	//맵 벗어나는 곳 제외
	for (auto& candPos : cand)
	{
		if (candPos.x >= 0 && candPos.x < 3 &&
			candPos.y >= 0 && candPos.y < 3)
		{
			res.pos.push_back(candPos);
		}
	}

	return res;
}

std::vector<Point> Heal::getEffectRange(Point pos) const
{
	return { Point(0,0) };
}

bool Heal::doSkill(Point pos, Hero * user, Hero * target, bool isAlly) const
{
	//아군에게만 영향
	if (!isAlly)
		return false;

	if (!isHeroInEffect(pos, target, isAlly))
		return false;

	target->damage(-4);

	return true;
}
