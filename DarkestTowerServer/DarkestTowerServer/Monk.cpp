#include "Monk.h"
#include "MonkAttack.h"

Monk::Monk(int idx)
	:Hero(HeroClass::MONK, 14 + rand() % 4, 4 + rand() % 3, idx)
{
	skills.push_back(SkillInfo(std::make_unique<MonkAttack>()));
}
