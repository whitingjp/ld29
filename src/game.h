#ifndef GAME_H
#define GAME_H

#include <entity/egg.h>
#include <entity/land.h>
#include <entity/worm.h>

typedef struct
{
	worm w;
	egg e;
	land* l;
} game;

void game_init(game* g);
void game_shutdown(game* g);
void game_update(game* g);
void game_draw(const game* g);

#endif // GAME_H