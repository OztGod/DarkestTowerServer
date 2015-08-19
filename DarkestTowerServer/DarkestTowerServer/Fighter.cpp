#include "Fighter.h"
#include "FighterAttack.h"

Fighter::Fighter()
	:Hero(HeroClass::FIGHTER, 17 + rand() % 4, 4 + rand() % 2)
{
	skills.push_back(SkillInfo(std::make_unique<FighterAttack>()));
}