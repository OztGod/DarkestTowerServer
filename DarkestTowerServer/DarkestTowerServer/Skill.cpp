#include "Skill.h"
#include "Hero.h"

bool Skill::isValidActPos(Point pos, const Map& map, int t) const
{
	Range res = getRange(pos, map, t);

	//range ���� ���� �־��� ��ǥ�� �ִ� -> �ùٸ� ��ġ��
	if (std::find(res.pos.begin(), res.pos.end(), pos) != res.pos.end())
	{
		return true;
	}

	return false;
}

bool Skill::isHeroInEffect(Point pos, const Hero * target, bool isAlly) const
{
	//�� �ʵ忡 ���� ��ų�̸� ally��, ��� �ʵ忡 ���� ��ų�̸� ally�� �ƴϿ��߸� ����ȴ�
	if (isMyField != isAlly)
		return false;

	std::vector<Point> attackPos = getEffectRange(pos);

	//��� ��ǥ ���� ��ǥ�� ��ȯ
	for (auto& p : attackPos)
	{
		p.x += pos.x;
		p.y += pos.y;
	}

	//hero ��ġ�� skill ���� �ٱ�
	if (std::find(attackPos.begin(), attackPos.end(), target->getPos()) == attackPos.end())
		return false;

	return true;
}
