#include "Thief.h"

Thief::Thief()
	:Hero(HeroClass::THIEF, 14 + rand() % 3, 5 + rand() % 3)
{
}
