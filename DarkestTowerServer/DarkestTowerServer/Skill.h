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

	//skill 해당 위치가 시전이 적절한 위치인지
	virtual bool isValidPos(Point pos) const = 0;

	//skill 시전 범위 리턴
	virtual Range getRange(Point pos) const = 0;

private:
	SkillType type;
	int level;
	int act;
	int cool;

};