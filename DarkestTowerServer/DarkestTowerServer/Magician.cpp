#include "Magician.h"
#include "ThunderStorm.h"
#include "FireBlast.h"
#include "IceArrow.h"

Magician::Magician(int idx)
	:Hero(HeroClass::MAGICIAN,10 + rand() % 3, 5 + rand() % 3, idx) 
{
	skills.push_back(SkillInfo(std::make_unique<ThunderStorm>()));
	skills.push_back(SkillInfo(std::make_unique<FireBlast>()));
	skills.push_back(SkillInfo(std::make_unique<IceArrow>()));
}
