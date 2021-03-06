#pragma once
#include "pcPacket.h"
#include "Types.h"
#include <vector>
#include <memory>
#include "Skill.h"
#include "State.h"

const int INVALID_POS = -1;

class Match;


struct SkillInfo
{
	SkillInfo(std::unique_ptr<Skill> skill_)
		:skill(std::move(skill_)), cool(0)
	{
	}

	std::unique_ptr<Skill> skill;
	int cool;
};

SkillInfo makeSkillInfo(SkillType type);

struct HeroInfo
{
	HeroClass type;
	int id;
	int maxHp;
	int maxAct;
	int level;
	int exp;
	int hpGrow;
	int actGrow;
	bool isValid = true;
	std::vector<SkillType> skillType;

	void levelup();
	void skillup();
};

HeroInfo makeRandomHeroInfo();

class Hero
{
	friend class Match;

public:
	Hero() = default;
	Hero(HeroClass type_, int maxHp_, int maxAct_, int idx_);

	void initByInfo(HeroInfo info_, int idx_);
	virtual ~Hero() = default;
	HeroClass getType() const { return type; }
	int getHp() const { return hp; }
	int getMaxHp() const { return maxHp; }
	int getAct() const { return act; }
	int getMaxAct() const { return maxAct; }
	int getLevel() const { return level; }
	Point getPos() const { return pos; }
	int getSkillNum() const { return skills.size(); }
	const Skill* getSkill(int idx) const { return skills[idx].skill.get(); }
	void setPos(Point pos_);
	bool move(Point pos_);
	bool consumeAct(int value);
	void damage(int value);
	void setSkillCool(int idx, int cool);
	int getSkillCool(int idx) const { return skills[idx].cool; }
	void turnUpdate();
	bool isDead() const { return !isAlive; }
	int getAttack() const;
	int getDefence() const;
	int getIdx() const { return idx; }

	void addState(std::unique_ptr<State>&& state);

protected:
	void dead();
	HeroClass type;
	int idx;
	int hp;
	int maxHp;
	int act;
	int maxAct;
	int level = 1;
	bool isAlive = true;
	Point pos;
	std::vector<SkillInfo> skills;
	std::vector<std::unique_ptr<State>> states;
};

std::unique_ptr<Hero> getHeroFromInfo(HeroInfo info, int idx);