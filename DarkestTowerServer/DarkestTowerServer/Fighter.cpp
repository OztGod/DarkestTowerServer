#include "Fighter.h"
#include "FighterAttack.h"
#include "Charge.h"

Fighter::Fighter(int idx)
	:Hero(HeroClass::FIGHTER, 17 + rand() % 4, 4 + rand() % 2, idx)
{
	skills.push_back(SkillInfo(std::make_unique<FighterAttack>()));
	skills.push_back(SkillInfo(std::make_unique<Charge>()));
}
