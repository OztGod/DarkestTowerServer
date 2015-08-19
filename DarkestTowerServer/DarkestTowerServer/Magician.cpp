#include "Magician.h"
#include "ThunderStorm.h"

Magician::Magician()
	:Hero(HeroClass::MAGICIAN,10 + rand() % 3, 5 + rand() % 3) 
{
	skills.push_back(SkillInfo(std::make_unique<ThunderStorm>()));
}
