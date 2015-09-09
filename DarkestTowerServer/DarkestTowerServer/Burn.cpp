#include "Burn.h"
#include "Hero.h"

void Burn::apply(Hero* hero, Hero* enemy, bool isAct)
{
}

void Burn::update(Hero* hero)
{
	duration--;

	if (duration == 0)
		end();

	hero->damage(damage);
}

void Burn::effectUpdate(std::vector<int> effect, UHeroVec & heros, bool isMyTurn)
{
}
