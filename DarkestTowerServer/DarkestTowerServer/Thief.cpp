#include "Thief.h"
#include "ThiefAttack.h"
#include "BackStep.h"

Thief::Thief(int idx)
	:Hero(HeroClass::THIEF, 14 + rand() % 3, 5 + rand() % 3, idx)
{
	skills.push_back(SkillInfo(std::make_unique<ThiefAttack>()));
	skills.push_back(SkillInfo(std::make_unique<BackStep>()));
}
