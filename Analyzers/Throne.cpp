#include "Throne.h"
#include "../Globals.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

Throne::Throne(Saiph *saiph) : Analyzer("Throne"), saiph(saiph) {
}

void Throne::analyze() {
	if (priority >= PRIORITY_THRONE_SIT)
		return;
	else if (saiph->world->player.hitpoints < 7 || (!saiph->world->player.shock_resistance && saiph->world->player.hitpoints < 31))
		return; // don't get killed by an electric chair
	else if (saiph->world->player.hitpoints - saiph->world->player.hitpoints_max > 5)
		return; // for the HP boost result
	int moves = 0;
	unsigned char dir = saiph->shortestPath(THRONE, false, &moves);
	if (dir == ILLEGAL_DIRECTION)
		return;
	else if (dir == NOWHERE)
		command = SIT;
	else
		command = dir;
	priority = PRIORITY_THRONE_SIT;
}
