#pragma once
#include "Types.h"
#include "pcPacket.h"
#include <vector>

class Hero;

class Skill
{
public:
	Skill(SkillType type_, int act_, int cool_)
		: type(type_), level(1), act(act_), cool(cool_)
	{ }
	virtual ~Skill() = default;

	int getLevel() const { return level; }
	SkillType getType() const { return type; }

	//skill �ش� ��ġ�� ������ ������ ��ġ����
	bool isValidActPos(Point pos) const;

	//�� ��ġ�� ������ �� �� �ִ� ��ġ����
	virtual bool isActEnable(Point pos) const = 0;

	//skill �� �� �ִ� ���� ����
	virtual Range getRange(Point pos) const = 0;

	//skill�� ȿ���� ���� ���� ����. ��� ��ǥ
	virtual std::vector<Point> getEffectRange(Point pos) const = 0;

	virtual void doSkill(Point pos, Hero* target, bool isAlly) = 0;

protected:
	bool isHeroInEffect(Point pos, Hero* target, bool isAlly);

	SkillType type;
	int level;
	int act;
	int cool;
};