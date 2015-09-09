#include "Archer.h"
#include "ArcherAttack.h"
#include "BackAttack.h"
#include "PenetrateShot.h"
#include "Snipe.h"

Archer::Archer(int idx)
	:Hero(HeroClass::ARCHER,10 + rand() % 3, 5 + rand() % 3, idx)
{
	skills.push_back(SkillInfo(std::make_unique<ArcherAttack>()));
	skills.push_back(SkillInfo(std::make_unique<BackAttack>()));
	skills.push_back(SkillInfo(std::make_unique<PenetrateShot>()));
	skills.push_back(SkillInfo(std::make_unique<Snipe>()));

}
