#include "Skill.h"
#include "Hero.h"

bool Skill::isValidActPos(Point pos, Hero* user, const UHeroVec& ally, const UHeroVec& enemy) const
{
	return isActEnable(pos, ally, enemy) && user->getAct() >= act;
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
