#include "Fighter.h"

Fighter::Fighter()
	:Hero(HeroClass::FIGHTER, 17 + rand() % 4, 4 + rand() % 2)
{
}
