#include "Skill.h"
#include "Hero.h"

bool Skill::isValidActPos(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	Range res = getRange(pos, ally, enemy);

	//range 범위 내에 주어진 좌표가 있다 -> 올바른 위치다
	if (std::find(res.pos.begin(), res.pos.end(), pos) != res.pos.end())
	{
		return true;
	}

	return false;
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
	for (int i = 0; target.size(); i++)
	{
		if (std::find(attackPos.begin(), attackPos.end(), target[i]->getPos()) != attackPos.end())
			res.push_back(i);
	}

	return res;
}
