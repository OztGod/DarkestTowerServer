#include "Monk.h"
#include "MonkAttack.h"

Monk::Monk()
	:Hero(HeroClass::MONK, 14 + rand() % 4, 4 + rand() % 3)
{
	skills.push_back(SkillInfo(std::make_unique<MonkAttack>()));
}
