#pragma once
#include "pcPacket.h"
#include "Types.h"
#include <vector>
#include <memory>
#include "Skill.h"

class Hero
{
public:
	Hero(HeroClass type_, int maxHp_, int maxAct_);
	virtual ~Hero() = default;
	HeroClass getType() const { return type; }
	int getHp() const { return hp; }
	int getMaxHp() const { return maxHp; }
	int getAct() const { return act; }
	int getMaxAct() const { return maxAct; }
	int getLevel() const { return level; }
	Point getPos() const { return pos; }
	int getSkillNum() const { return skills.size(); }
	const Skill* getSkill(int idx) const { return skills[idx].get(); }
	bool setPos(Point pos_);
	bool consumeAct(int value);

private:
	HeroClass type;
	int hp;
	int maxHp;
	int act;
	int maxAct;
	int level = 1;
	Point pos;
	std::vector<std::unique_ptr<Skill> > skills;
};

std::unique_ptr<Hero> getRandomHero();