#include "Priest.h"

Priest::Priest()
	:Hero(HeroClass::PRIEST, 10 + rand() % 3, 5 + rand() % 2)
{
}
