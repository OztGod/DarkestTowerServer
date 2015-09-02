#include "Hero.h"
#include "Archer.h"
#include "Fighter.h"
#include "Magician.h"
#include "Monk.h"
#include "Priest.h"
#include "Thief.h"

Hero::Hero(HeroClass type_, int maxHp_, int maxAct_)
	: type(type_), maxHp(maxHp_), hp(maxHp_), maxAct(maxAct_), act(maxAct_)
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

std::unique_ptr<Hero> getRandomHero()
{
	std::unique_ptr<Hero> hero = nullptr;
	int heroNum = static_cast<int>(HeroClass::NUM);
	HeroClass heroClass = static_cast<HeroClass>(rand() % 3 /*heroNum*/);

	switch (heroClass)
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

	return hero;
}
