#include "Archer.h"
#include "ArcherAttack.h"

Archer::Archer()
	:Hero(HeroClass::ARCHER,10 + rand() % 3, 5 + rand() % 3)
{
	skills.push_back(SkillInfo(std::make_unique<ArcherAttack>()));
}
