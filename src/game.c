#include "game.h"

#include <stdlib.h>

void game_init(game* g)
{
	g->l = malloc(sizeof(land));
	land_zero(g->l);
	g->w = worm_zero(g->l);
	g->e = egg_zero(g->l);
	whitgl_fvec drill_pos = {g->l->size.x/2-25, 0};
	g->drill = driller_zero(drill_pos);
}
void game_shutdown(game* g)
{
	free(g->l);
	free(g);
}
void game_update(game* g)
{
	land_update(g->l);
	g->w = worm_update(g->w, g->l);
	g->e = egg_update(g->e, g->l);
	g->drill = driller_update(g->drill, g->l, g->e);
	whitgl_fcircle splat = whitgl_fcircle_zero;
	splat.pos = g->w.segments[5]; // can't do 0 because it mucks up land_filled
	splat.size = 40;
	land_splat(g->l, splat);
	splat.pos = g->drill.pos;
	splat.size = 5;
	land_splat(g->l, splat);
}
void game_draw(const game* g)
{
	land_draw(g->l);
	worm_draw(g->w);
	egg_draw(g->e);
	driller_draw(g->drill);
}