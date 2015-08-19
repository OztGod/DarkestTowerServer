#include "Archer.h"

Archer::Archer()
	:Hero(HeroClass::ARCHER,10 + rand() % 3, 5 + rand() % 3)
{
}
