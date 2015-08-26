#include "State.h"

void State::end()
{
	damage = 0;
	hp = 0;
	act = 0;
	attack = 0;
	defence = 0;
	duration = 0;
	ended = true;
}
