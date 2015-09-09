#include "Ice.h"
#include "Hero.h"

void Ice::apply(Hero * hero, Hero * enemy, bool isAct)
{
}

void Ice::update(Hero * hero)
{
	hero->consumeAct(act);

	duration--;

	if (duration == 0)
		end();
}

void Ice::effectUpdate(std::vector<int> effect, UHeroVec & heros, bool isMyTurn)
{
}
