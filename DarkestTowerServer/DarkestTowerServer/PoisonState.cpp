#include "PoisonState.h"
#include "Hero.h"

void PoisonState::apply(Hero * hero, Hero * enemy, bool isAct)
{
}

void PoisonState::update(Hero * hero)
{
	duration--;

	if (duration == 0)
		end();

	hero->damage(damage);
}

void PoisonState::effectUpdate(std::vector<int> effect, UHeroVec & heros, bool isMyTurn)
{
}
