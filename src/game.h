#ifndef GAME_H
#define GAME_H

#include <entity/driller.h>
#include <entity/egg.h>
#include <entity/human.h>
#include <entity/land.h>
#include <entity/worm.h>

#include <damage.h>

#define MAX_HUMANS (16)
#define MAX_WORMS (8)
typedef struct
{
	ld29_worm worms[MAX_WORMS];
	ld29_egg egg;
	ld29_land* land;
	ld29_driller drill;
	ld29_human humans[MAX_HUMANS];
	int player;
	whitgl_float transition;
	whitgl_float throb;
} ld29_game;

void game_init(ld29_game* g);
void game_shutdown(ld29_game* g);
void game_update(ld29_game* g, whitgl_ivec screen_size);
void game_do_damage(ld29_game* g, ld29_damage d);
void game_draw(const ld29_game* g, whitgl_ivec screen_size);

void game_blast();

#endif // GAME_H