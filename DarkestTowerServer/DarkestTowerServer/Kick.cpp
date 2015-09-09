#include "Kick.h"
#include "Hero.h"

Kick::Kick()
	:Skill(SkillType::MONK_KICK, 2, 3, false)
{
}

bool Kick::isActEnable(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	if (pos.y == 0)
		return true;

	return false;
}

Range Kick::getRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	Range res;
	res.isMyField = isMyField;
	res.pos.push_back({ 2 - pos.x,0 });
	
	return res;
}

std::vector<Point> Kick::getEffectRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	return{ Point(0,0) };
}

bool Kick::doSkill(Point pos, Hero * user, Hero * target, const UHeroVec & ally, const UHeroVec & enemy) const
{
	for (auto& h : enemy)
	{
		if (h->getPos().x == target->getPos().x && h->getPos().y > target->getPos().y)
		{
			h->setPos({ h->getPos().x, h->getPos().y - 1 });
		}
	}

	//맨 뒤로 이동
	target->setPos({ target->getPos().x, 2 });

	return true;
}
