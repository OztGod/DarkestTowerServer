#include "Hero.h"
#include "Archer.h"
#include "Fighter.h"
#include "Magician.h"
#include "Monk.h"
#include "Priest.h"
#include "Thief.h"
#include "ArcherAttack.h"
#include "BackAttack.h"
#include "PenetrateShot.h"
#include "Snipe.h"
#include "Charge.h"
#include "FighterAttack.h"
#include "FireBlast.h"
#include "IceArrow.h"
#include "ThunderStorm.h"
#include "MonkAttack.h"
#include "Kick.h"
#include "Heal.h"
#include "PriestAttack.h"
#include "BackStep.h"
#include "Poison.h"
#include "ThiefAttack.h"

Hero::Hero(HeroClass type_, int maxHp_, int maxAct_, int idx_)
	: type(type_), maxHp(maxHp_), hp(maxHp_), maxAct(maxAct_), act(maxAct_), idx(idx_)
{
}

void Hero::initByInfo(HeroInfo info_, int idx_)
{
	type = info_.type;
	maxHp = info_.maxHp;
	maxAct = info_.maxAct;
	hp = maxHp;
	act = maxAct;
	idx = idx_;

	for (auto& skill : info_.skillType)
	{
		skills.push_back(makeSkillInfo(skill));
	}
}

void Hero::setPos(Point pos_)
{
	pos = pos_;
}

bool Hero::move(Point pos_)
{
	Point prevPos = pos;

	int delta = abs(pos_.x - prevPos.x + pos_.y - prevPos.y);
	if (delta != 1)
		return false;

	setPos(pos_);

	return true;
}

bool Hero::consumeAct(int value)
{
	if (act < value)
		return false;

	act -= value;

	return true;
}

void Hero::damage(int value)
{
	hp -= value;

	if (hp >= maxHp)
	{
		hp = maxHp;
	}

	if (hp <= 0)
	{
		dead();
	}
}

void Hero::setSkillCool(int idx, int cool)
{
	skills[idx].cool = cool;
}

void Hero::turnUpdate()
{
	act = maxAct;

	//state 반영
	for (auto& state : states)
	{
		state->update(this);
	}

	for (auto& skill : skills)
	{
		if (skill.cool > 0)
			skill.cool--;
	}
}

int Hero::getAttack() const
{
	int attack = 0;

	for (auto& state : states)
	{
		attack += state->getAttack();
	}

	return attack;
}

int Hero::getDefence() const
{
	int defence = 0;

	for (auto& state : states)
	{
		defence += state->getDefence();
	}
	
	return defence;
}

void Hero::addState(std::unique_ptr<State>&& state)
{
	states.push_back(std::move(state));
}

void Hero::dead()
{
	isAlive = false;
	pos.x = INVALID_POS;
	pos.y = INVALID_POS;
}

std::unique_ptr<Hero> getHeroFromInfo(HeroInfo info, int idx)
{
	std::unique_ptr<Hero> hero = nullptr;

	switch (info.type)
	{
	case HeroClass::FIGHTER:
		hero = std::make_unique<Fighter>();
		break;
	case HeroClass::ARCHER:
		hero = std::make_unique<Archer>();
		break;
	case HeroClass::MAGICIAN:
		hero = std::make_unique<Magician>();
		break;
	case HeroClass::MONK:
		hero = std::make_unique<Monk>();
		break;
	case HeroClass::PRIEST:
		hero = std::make_unique<Priest>();
		break;
	case HeroClass::THIEF:
		hero = std::make_unique<Thief>();
		break;
	}

	hero->initByInfo(info, idx);

	return hero;
}

HeroInfo makeRandomHeroInfo()
{
	HeroClass type = static_cast<HeroClass>(rand() % static_cast<int>(HeroClass::NUM));
	HeroInfo info;
	std::vector<SkillType> skills;

	info.type = type;
	info.level = 0;

	switch (type)
	{
	case HeroClass::ARCHER:
		info.hpGrow = 2;
		info.actGrow = 2;
		info.maxHp = 10;
		info.maxAct = 5;
		info.skillType.push_back(SkillType::ARCHER_ATTACK);

		skills.push_back(SkillType::ARCHER_BACK_ATTACK);
		skills.push_back(SkillType::ARCHER_PENETRATE_SHOT);
		skills.push_back(SkillType::ARCHER_SNIPE);
		break;

	case HeroClass::FIGHTER:
		info.hpGrow = 3;
		info.actGrow = 2;
		info.maxHp = 17;
		info.maxAct = 5;
		info.skillType.push_back(SkillType::FIGHTER_ATTACK);

		skills.push_back(SkillType::FIGHTER_CHARGE);
		break;

	case HeroClass::MAGICIAN:
		info.hpGrow = 2;
		info.actGrow = 2;
		info.maxHp = 10;
		info.maxAct = 5;
		info.skillType.push_back(SkillType::MAGICIAN_FIRE_BLAST);

		skills.push_back(SkillType::MAGICIAN_ICE_ARROW);
		skills.push_back(SkillType::MAGICIAN_THUNDER_STORM);
		break;

	case HeroClass::MONK:
		info.hpGrow = 3;
		info.actGrow = 2;
		info.maxHp = 14;
		info.maxAct = 4;
		info.skillType.push_back(SkillType::MONK_ATTACK);

		skills.push_back(SkillType::MONK_KICK);
		break;

	case HeroClass::PRIEST:
		info.hpGrow = 2;
		info.actGrow = 1;
		info.maxHp = 10;
		info.maxAct = 5;
		info.skillType.push_back(SkillType::PRIEST_HEAL);

		skills.push_back(SkillType::PRIEST_ATTACK);
		break;

	case HeroClass::THIEF:
		info.hpGrow = 2;
		info.actGrow = 2;
		info.maxHp = 14;
		info.maxAct = 5;
		info.skillType.push_back(SkillType::THIEF_ATTACK);

		skills.push_back(SkillType::THIEF_BACK_STEP);
		skills.push_back(SkillType::THIEF_POISON);
		break;
	}

	info.maxHp += rand() % (1 + info.hpGrow);
	info.maxAct += rand() % (1 + info.actGrow);

	info.skillType.push_back(skills[rand() % skills.size()]);

	return info;
}

SkillInfo makeSkillInfo(SkillType type)
{
	SkillInfo info(nullptr);

	switch (type)
	{
	case SkillType::ARCHER_ATTACK:
		info.skill = std::make_unique<ArcherAttack>();
		break;
	case SkillType::ARCHER_BACK_ATTACK:
		info.skill = std::make_unique<BackAttack>();
		break;
	case SkillType::ARCHER_PENETRATE_SHOT:
		info.skill = std::make_unique<PenetrateShot>();
		break;
	case SkillType::ARCHER_SNIPE:
		info.skill = std::make_unique<Snipe>();
		break;
	case SkillType::FIGHTER_ATTACK:
		info.skill = std::make_unique<FighterAttack>();
		break;
	case SkillType::FIGHTER_CHARGE:
		info.skill = std::make_unique<Charge>();
		break;
	case SkillType::MAGICIAN_FIRE_BLAST:
		info.skill = std::make_unique<FireBlast>();
		break;
	case SkillType::MAGICIAN_ICE_ARROW:
		info.skill = std::make_unique<IceArrow>();
		break;
	case SkillType::MAGICIAN_THUNDER_STORM:
		info.skill = std::make_unique<ThunderStorm>();
		break;
	case SkillType::MONK_ATTACK:
		info.skill = std::make_unique<MonkAttack>();
		break;
	case SkillType::MONK_KICK:
		info.skill = std::make_unique<Kick>();
		break;
	case SkillType::PRIEST_ATTACK:
		info.skill = std::make_unique<PriestAttack>();
		break;
	case SkillType::PRIEST_HEAL:
		info.skill = std::make_unique<Heal>();
		break;
	case SkillType::THIEF_ATTACK:
		info.skill = std::make_unique<ThiefAttack>();
		break;
	case SkillType::THIEF_BACK_STEP:
		info.skill = std::make_unique<BackStep>();
		break;
	case SkillType::THIEF_POISON:
		info.skill = std::make_unique<Poison>();
		break;
	}

	return info;
}

void HeroInfo::levelup()
{
	while (exp > level * level * 10)
	{
		exp -= level * level * 10;
		level++;

		int t = rand() % 3;

		switch (t)
		{
		case 0: //HP GROW
			maxHp += hpGrow;
			break;
		case 1: //AP GROW
			maxAct += actGrow;
			break;
		case 2: //SKILL GROW
			skillup();
			break;
		}
	}
}

void HeroInfo::skillup()
{
	std::vector<SkillType> types;

	//현재는 스킬 레벨업은 무시
	//새 스킬 습득 가능한 직업에 한해 새 스킬 습득하게 해줌
	switch (type)
	{
	case HeroClass::ARCHER:
		types.push_back(SkillType::ARCHER_BACK_ATTACK);
		types.push_back(SkillType::ARCHER_PENETRATE_SHOT);
		types.push_back(SkillType::ARCHER_SNIPE);
		break;
	case HeroClass::MAGICIAN:
		types.push_back(SkillType::MAGICIAN_ICE_ARROW);
		types.push_back(SkillType::MAGICIAN_THUNDER_STORM);
		break;
	case HeroClass::THIEF:
		types.push_back(SkillType::THIEF_BACK_STEP);
		types.push_back(SkillType::THIEF_POISON);
		break;
	}

	//모든 스킬 다 습득한 경우
	if (skillType.size() > types.size())
		return;

	int idx = rand() % types.size();

	while (std::find(skillType.begin(), skillType.end(), types[idx]) != skillType.end())
	{
		idx = rand() % types.size();
	}

	skillType.push_back(types[idx]);
}
