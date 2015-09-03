#include "Skill.h"
#include "Hero.h"

bool Skill::isValidActPos(Point pos, Hero* user, const UHeroVec& ally, const UHeroVec& enemy) const
{
	return isActEnable(pos, ally, enemy) && user->getAct() >= act;
}

std::vector<int> Skill::getHeroInEffect(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	//내 필드에 쓰는 스킬이면 ally만, 상대 필드에 쓰는 스킬이면 ally가 아니여야만 적용된다

	std::vector<Point> attackPos = getEffectRange(pos, ally, enemy);

	//상대 좌표 절대 좌표로 변환
	for (auto& p : attackPos)
	{
		p.x += pos.x;
		p.y += pos.y;
	}

	std::vector<int> res;
	const UHeroVec& target = isMyField ? ally : enemy;

	//hero 위치가 skill 범위 내부인 것 리턴.
	for (int i = 0; i < target.size(); i++)
	{
		if (std::find(attackPos.begin(), attackPos.end(), target[i]->getPos()) != attackPos.end())
			res.push_back(i);
	}

	return res;
}

void Skill::damage(int value, Hero* user, Hero* target) const
{
	int d = value + user->getAttack() - target->getDefence();

	if (d <= 0)
		d = 1;

	target->damage(d);
}
