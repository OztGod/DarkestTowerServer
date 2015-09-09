#include "BackStep.h"
#include "Hero.h"

BackStep::BackStep()
	:Skill(SkillType::THIEF_BACK_STEP, 3, 3, false)
{
}

bool BackStep::isActEnable(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	if (pos.y == 0)
		return true;

	return false;
}

Range BackStep::getRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	Range res;

	res.isMyField = isMyField;
	res.pos.push_back({ 2 - pos.x,1 });
	res.pos.push_back({ 2 - pos.x,2 });

	return res;
}

std::vector<Point> BackStep::getEffectRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	return{ Point(0,0) };
}

bool BackStep::doSkill(Point pos, Hero * user, Hero * target, const UHeroVec & ally, const UHeroVec & enemy) const
{
	damage(2, user, target);

	for (auto& h : ally)
	{
		if (h->getPos().x == user->getPos().x && h->getPos().y == user->getPos().y + 1)
		{
			h->setPos({ h->getPos().x, h->getPos().y - 1 });
		}
	}

	user->setPos({ user->getPos().x,user->getPos().y + 1 });

	return true;
}
