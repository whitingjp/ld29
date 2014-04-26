#include "game.h"

#include <stdlib.h>

void game_init(ld29_game* g)
{
	g->land = malloc(sizeof(ld29_land));
	land_zero(g->land);
	g->worm = worm_zero(g->land);
	g->egg = egg_zero(g->land);
	whitgl_fvec drill_pos = {g->land->size.x/2-25, 0};
	g->drill = driller_zero(drill_pos);
}
void game_shutdown(ld29_game* g)
{
	free(g->land);
	free(g);
}
void game_update(ld29_game* g)
{
	land_update(g->land);
	g->worm = worm_update(g->worm, g->land);
	g->egg = egg_update(g->egg, g->land);
	g->drill = driller_update(g->drill, g->land, g->egg);
	if(g->drill.attack.type != DAMAGE_NONE)
	{
		game_do_damage(g, g->drill.attack);
		whitgl_fvec drill_pos = {g->land->size.x/2-25, 0};
		g->drill = driller_zero(drill_pos);
	}
	whitgl_fcircle splat = whitgl_fcircle_zero;
	splat.pos = g->worm.segments[5]; // can't do 0 because it mucks up land_filled
	splat.size = 40;
	land_splat(g->land, splat);
	splat.pos = g->drill.pos;
	splat.size = 5;
	land_splat(g->land, splat);
}
void game_do_damage(ld29_game* g, ld29_damage damage)
{
	(void)g;
	(void)damage;
}
void game_draw(const ld29_game* g)
{
	land_draw(g->land);
	worm_draw(g->worm);
	egg_draw(g->egg);
	driller_draw(g->drill);
}