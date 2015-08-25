#include "Charge.h"
#include "Hero.h"

Charge::Charge()
	:Skill(SkillType::FIGHTER_CHARGE, 3, 3, false)
{
}

bool Charge::isActEnable(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	if (pos.y == 0)
		return false;

	return true;
}

Range Charge::getRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	Range res;

	res.isMyField = isMyField;
	res.pos.push_back(Point(2 - pos.x, 0));
	return res;
}

std::vector<Point> Charge::getEffectRange(Point pos, const UHeroVec & ally, const UHeroVec & enemy) const
{
	return{ Point(0,0) };
}

bool Charge::doSkill(Point pos, Hero * user, Hero * target, const UHeroVec & ally, const UHeroVec & enemy) const
{
	target->damage(1);
	for (auto& h : ally)
	{
		//���� �ٿ� �ְ� user���� �տ� ������ ��ĭ�� �ڷ� �δ�
		if (h->getPos().x == user->getPos().x && h->getPos().y < user->getPos().y)
		{
			h->setPos({ h->getPos().x, h->getPos().y + 1 });
		}
	}

	//�� ������ �̵�
	user->setPos({ user->getPos().x, 0 });

	return true;
}
