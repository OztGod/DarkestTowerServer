#include "Thief.h"
#include "ThiefAttack.h"

Thief::Thief()
	:Hero(HeroClass::THIEF, 14 + rand() % 3, 5 + rand() % 3)
{
	skills.push_back(SkillInfo(std::make_unique<ThiefAttack>()));
}
