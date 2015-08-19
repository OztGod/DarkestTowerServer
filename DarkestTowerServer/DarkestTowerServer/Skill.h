#pragma once
#include "Types.h"
#include "pcPacket.h"
#include <vector>
#include "Map.h"

class Hero;

class Skill
{
public:
	Skill(SkillType type_, int act_, int cool_, bool isMyField_)
		: type(type_), level(1), act(act_), cool(cool_), isMyField(isMyField_)
	{ }
	virtual ~Skill() = default;

	int getLevel() const { return level; }
	int getAct() const { return act; }
	int getCool() const { return cool; }
	SkillType getType() const { return type; }

	//skill �ش� ��ġ�� ������ ������ ��ġ����
	bool isValidActPos(Point pos, const Map& map, int t) const;

	//�ش� ������ ��ų ȿ�� ���� ���� �����ϴ���
	bool isHeroInEffect(Point pos, const Hero* target, bool isAlly) const;

	//�� ��ġ�� ������ �� �� �ִ� ��ġ����
	virtual bool isActEnable(Point pos, const Map& map, int t) const = 0;

	//skill �� �� �ִ� ���� ����
	virtual Range getRange(Point pos, const Map& map, int t) const = 0;

	//skill�� ȿ���� ���� ���� ����. ��� ��ǥ. pos�� �����ϴ� ������ ��ġ.
	virtual std::vector<Point> getEffectRange(Point pos) const = 0;

	virtual bool doSkill(Point pos, Hero* user, Hero* target, bool isAlly) const = 0;

protected:

	SkillType type;
	int level;
	int act;
	int cool;
	bool isMyField;
};