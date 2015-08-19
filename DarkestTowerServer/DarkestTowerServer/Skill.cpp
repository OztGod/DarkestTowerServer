#include "Skill.h"
#include "Hero.h"

bool Skill::isValidActPos(Point pos) const
{
	Range res = getRange(pos);

	//range 범위 내에 주어진 좌표가 있다 -> 올바른 위치다
	if (std::find(res.pos.begin(), res.pos.end(), pos) != res.pos.end())
	{
		return true;
	}

	return false;
}

bool Skill::isHeroInEffect(Point pos, const Hero * target, bool isAlly) const
{
	std::vector<Point> attackPos = getEffectRange(pos);

	//상대 좌표 절대 좌표로 변환
	for (auto& p : attackPos)
	{
		p.x += pos.x;
		p.y += pos.y;
	}

	//hero 위치가 skill 범위 바깥
	if (std::find(attackPos.begin(), attackPos.end(), target->getPos()) == attackPos.end())
		return false;

	return true;
}
