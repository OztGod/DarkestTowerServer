#include "ThunderStorm.h"
#include "Hero.h"

ThunderStorm::ThunderStorm()
	:Skill(SkillType::MAGICIAN_THUNDER_STORM, 5, 3, false)
{
}

bool ThunderStorm::isActEnable(Point pos, const Map& map, int t) const
{
	if (pos.y == 2)
		return true;

	return false;
}

Range ThunderStorm::getRange(Point pos, const Map& map, int t) const
{
	Range res;

	res.isMyField = isMyField;
	res.pos.push_back(Point(2 - pos.x, 0));

	return res;
}

std::vector<Point> ThunderStorm::getEffectRange(Point pos) const
{
	return
	{
		Point(pos.x - 2, 0),
		Point(pos.x - 1, 0),
		Point(pos.x, 0)
	};
}

bool ThunderStorm::doSkill(Point pos, Hero* user, Hero * target, bool isAlly) const
{
	//적군에게만 영향
	if (isAlly)
		return false;

	if (!isHeroInEffect(pos, target, isAlly))
		return false;

	target->damage(5);

	return true;
}
