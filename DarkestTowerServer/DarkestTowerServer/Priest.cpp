#include "Priest.h"
#include "Heal.h"
#include "PriestAttack.h"

Priest::Priest(int idx)
	:Hero(HeroClass::PRIEST, 10 + rand() % 3, 5 + rand() % 2, idx)
{
	skills.push_back(SkillInfo(std::make_unique<Heal>()));
	skills.push_back(SkillInfo(std::make_unique<PriestAttack>()));
}
