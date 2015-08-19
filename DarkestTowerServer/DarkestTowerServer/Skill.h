#pragma once
#include "Types.h"
#include "pcPacket.h"
#include <vector>

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
	virtual bool isValidPos(Point pos) const = 0;

	//skill ���� ���� ����
	virtual Range getRange(Point pos) const = 0;

private:
	SkillType type;
	int level;
	int act;
	int cool;

};