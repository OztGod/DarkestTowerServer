#include "Skill.h"
#include "Hero.h"

bool Skill::isValidActPos(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	Range res = getRange(pos, ally, enemy);

	//range ���� ���� �־��� ��ǥ�� �ִ� -> �ùٸ� ��ġ��
	if (std::find(res.pos.begin(), res.pos.end(), pos) != res.pos.end())
	{
		return true;
	}

	return false;
}

std::vector<int> Skill::getHeroInEffect(Point pos, const UHeroVec& ally, const UHeroVec& enemy) const
{
	//�� �ʵ忡 ���� ��ų�̸� ally��, ��� �ʵ忡 ���� ��ų�̸� ally�� �ƴϿ��߸� ����ȴ�

	std::vector<Point> attackPos = getEffectRange(pos, ally, enemy);

	//��� ��ǥ ���� ��ǥ�� ��ȯ
	for (auto& p : attackPos)
	{
		p.x += pos.x;
		p.y += pos.y;
	}

	std::vector<int> res;
	const UHeroVec& target = isMyField ? ally : enemy;

	//hero ��ġ�� skill ���� ������ �� ����.
	for (int i = 0; target.size(); i++)
	{
		if (std::find(attackPos.begin(), attackPos.end(), target[i]->getPos()) != attackPos.end())
			res.push_back(i);
	}

	return res;
}
