#include "BackAttack.h"
#include "Hero.h"

BackAttack::BackAttack()
	: Skill(SkillType::ARCHER_BACK_ATTACK, 3, 3, false)
{
}

bool BackAttack::isActEnable(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	if (pos.y == 0)
		return true;

	return false;
}

Range BackAttack::getRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	Range res;

	res.isMyField = isMyField;
	res.pos.push_back({ 2 - pos.x,0 });

	return res;
}

std::vector<Point> BackAttack::getEffectRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	return{ Point(0,0) };
}

bool BackAttack::doSkill(Point pos, Hero * user, Hero * target, const UHeroVec & ally, const UHeroVec & enemy) const
{
	damage(1, user, target);

	for (auto& e : enemy)
	{
		if (e->getPos().x == target->getPos().x && e->getPos().y == target->getPos().y + 1)
		{
			e->setPos({ e->getPos().x, e->getPos().y - 1 });
		}
	}

	target->setPos({ target->getPos().x, target->getPos().y + 1 });

	for (auto& h : ally)
	{
		if (h->getPos().x == user->getPos().x && h->getPos().y == user->getPos().y + 1)
		{
			h->setPos({ h->getPos().x, h->getPos().y - 1 });
		}
	}
	user->setPos({ user->getPos().x, user->getPos().y + 1 });

	return true;
}
