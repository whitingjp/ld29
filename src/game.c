#include "game.h"

#include <stdlib.h>

void game_init(ld29_game* g)
{
	g->land = malloc(sizeof(ld29_land));
	land_zero(g->land);
	g->worm = worm_zero(g->land);
	g->egg = egg_zero(g->land);
	whitgl_fcircle splat = whitgl_fcircle_zero;
	splat.pos = g->egg.pos;
	splat.size = 20;
	land_splat(g->land, splat);
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
	splat.size = 6;
	land_splat(g->land, splat);
	splat.pos = g->drill.pos;
	splat.size = 2;
	land_splat(g->land, splat);
}
void game_do_damage(ld29_game* g, ld29_damage damage)
{
	int i;
	whitgl_fvec start_off = {-64, 0};
	whitgl_fvec p = whitgl_fvec_add(damage.pos, start_off);
	whitgl_fvec inc = {4, 0};
	for(i=0; i<32; i++)
	{
		whitgl_fcircle splat = whitgl_fcircle_zero;
		splat.pos = p;
		splat.size = 5;
		land_splat(g->land, splat);
		whitgl_float sqmag = whitgl_fvec_sqmagnitude(whitgl_fvec_sub(g->egg.pos, p));
		if(sqmag < 16)
			g->egg.dead = true;
		p = whitgl_fvec_add(p, inc);
	}
	(void)g;
	(void)damage;
}
void game_draw(const ld29_game* g, whitgl_ivec screen_size)
{
	whitgl_ivec camera = whitgl_ivec_inverse(whitgl_fvec_to_ivec(g->worm.segments[0]));
	camera.x += screen_size.x/2;
	camera.y += screen_size.y/2;
	int wrap_dir = 0;
	if(camera.x > 0)
		wrap_dir = -1;
	if(camera.x < screen_size.x-g->land->size.x)
		wrap_dir = 1;
	if(camera.y > 0)
		camera.y = 0;
	if(camera.y < screen_size.y-g->land->size.y)
		camera.y = screen_size.y-g->land->size.y;

	land_draw(g->land, camera);
	worm_draw(g->worm, camera);
	egg_draw(g->egg, camera);
	driller_draw(g->drill, camera);

	camera.x += g->land->size.x*wrap_dir;

	land_draw(g->land, camera);
	worm_draw(g->worm, camera);
	egg_draw(g->egg, camera);
	driller_draw(g->drill, camera);
}