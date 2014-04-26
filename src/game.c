#include "game.h"

#include <stdlib.h>

void game_init(game* g)
{
	g->l = malloc(sizeof(land));
	land_zero(g->l);
	g->w = worm_zero(g->l);
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
	whitgl_fcircle splat = whitgl_fcircle_zero;
	splat.pos = g->w.segments[5]; // can't do 0 because it mucks up land_filled
	splat.size = 40;
	land_splat(g->l, splat);	
}
void game_draw(const game* g)
{
	land_draw(g->l);
	worm_draw(g->w);
}