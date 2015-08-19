#include "Monk.h"

Monk::Monk()
	:Hero(HeroClass::MONK, 14 + rand() % 4, 4 + rand() % 3)
{
}
