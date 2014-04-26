#ifndef GAME_H
#define GAME_H

#include <entity/driller.h>
#include <entity/egg.h>
#include <entity/land.h>
#include <entity/worm.h>

#include <damage.h>

typedef struct
{
	ld29_worm worm;
	ld29_egg egg;
	ld29_land* land;
	ld29_driller drill;
} ld29_game;

void game_init(ld29_game* g);
void game_shutdown(ld29_game* g);
void game_update(ld29_game* g);
void game_do_damage(ld29_game* g, ld29_damage d);
void game_draw(const ld29_game* g, whitgl_ivec screen_size);

#endif // GAME_H