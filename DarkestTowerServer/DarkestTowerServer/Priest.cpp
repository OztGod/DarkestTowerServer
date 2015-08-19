#include "Priest.h"
#include "Heal.h"

Priest::Priest()
	:Hero(HeroClass::PRIEST, 10 + rand() % 3, 5 + rand() % 2)
{
	skills.push_back(SkillInfo(std::make_unique<Heal>()));
}
