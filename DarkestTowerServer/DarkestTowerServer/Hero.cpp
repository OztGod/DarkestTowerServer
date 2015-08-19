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

bool Hero::setPos(Point pos_)
{
	Point prevPos = pos;

	int delta = abs(pos_.x - prevPos.x + pos_.y - prevPos.y);
	if (delta != 1)
		return false;

	pos = pos_;

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
}

std::unique_ptr<Hero> getRandomHero()
{
	std::unique_ptr<Hero> hero = nullptr;
	int heroNum = static_cast<int>(HeroClass::NUM);
	HeroClass heroClass = static_cast<HeroClass>(rand() % heroNum);

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
