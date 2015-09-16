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

	//state ¹Ý¿µ
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

std::unique_ptr<Hero> getRandomHero(int idx)
{
	std::unique_ptr<Hero> hero = nullptr;
	int heroNum = static_cast<int>(HeroClass::NUM);
	HeroClass heroClass = static_cast<HeroClass>(rand() % heroNum);

	switch (heroClass)
	{
	case HeroClass::FIGHTER:
		hero = std::make_unique<Fighter>(idx);
		break;
	case HeroClass::ARCHER:
		hero = std::make_unique<Archer>(idx);
		break;
	case HeroClass::MAGICIAN:
		hero = std::make_unique<Magician>(idx);
		break;
	case HeroClass::MONK:
		hero = std::make_unique<Monk>(idx);
		break;
	case HeroClass::PRIEST:
		hero = std::make_unique<Priest>(idx);
		break;
	case HeroClass::THIEF:
		hero = std::make_unique<Thief>(idx);
		break;
	}

	return hero;
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