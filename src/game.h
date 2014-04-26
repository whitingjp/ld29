#ifndef GAME_H
#define GAME_H

#include <entity/driller.h>
#include <entity/egg.h>
#include <entity/land.h>
#include <entity/worm.h>

#include <damage.h>

typedef struct
{
	worm w;
	egg e;
	land* l;
	driller drill;
} game;

void game_init(game* g);
void game_shutdown(game* g);
void game_update(game* g);
void game_do_damage(game* g, damage d);
void game_draw(const game* g);

#endif // GAME_H