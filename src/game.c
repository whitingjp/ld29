#include "game.h"

#include <stdlib.h>

#include <whitgl/sound.h>
#include <whitgl/sys.h>

#include <sounds.h>

typedef struct
{
	whitgl_float timer;
	whitgl_fcircle circle;
} ld29_damage_display;
ld29_damage_display ld29_damage_display_zero = {0, { {0,0}, 0}};

#define MAX_DISPLAYED_DAMAGES (128)
ld29_damage_display display_damages[MAX_DISPLAYED_DAMAGES];

void game_init(ld29_game* g)
{
	int i;
	g->land = malloc(sizeof(ld29_land));
	land_zero(g->land);
	for(i=0; i<MAX_WORMS; i++)
		g->worms[i] = worm_zero(g->land);
	g->egg = egg_zero();
	whitgl_fvec drill_pos = {whitgl_randint(g->land->size.x), 0};
	g->drill = driller_zero(drill_pos);
	for(i=0; i<MAX_DISPLAYED_DAMAGES; i++)
		display_damages[i] = ld29_damage_display_zero;
	for(i=0; i<MAX_HUMANS; i++)
	{
		whitgl_fvec human_pos = {whitgl_randint(g->land->size.x), 0};
		g->humans[i] = human_zero(human_pos);
	}
	g->player = 0;
}
void game_shutdown(ld29_game* g)
{
	free(g->land);
	free(g);
}
void game_update(ld29_game* g, whitgl_ivec screen_size)
{
	int i;
	land_update(g->land);
	for(i=0; i<MAX_WORMS; i++)
	{
		g->worms[i] = worm_update(g->worms[i], g->land, i==g->player);
		if(g->worms[i].pregnancy == -1)
		{
			g->egg = egg_zero();
			g->egg.dead = false;
			g->egg.pos = g->worms[i].segments[g->worms[i].num_segments-1];
			g->worms[i].pregnancy = 0;
		}
	}
	g->egg = egg_update(g->egg, g->land);
	g->drill = driller_update(g->drill, g->land, g->worms[g->player].segments[0], whitgl_ivec_to_fvec(screen_size));
	for(i=0; i<MAX_HUMANS; i++)
		g->humans[i] = human_update(g->humans[i], g->land);
	for(i=0; i<MAX_HUMANS; i++)
	{
		if(!g->humans[i].alive)
		{
			whitgl_fvec human_pos = {whitgl_randint(g->land->size.x), 0};
			g->humans[i] = human_zero(human_pos);
		}
		int j;
		for(j=0; j<MAX_WORMS; j++)
		{
			whitgl_fvec dist = whitgl_fvec_sub(g->humans[i].pos, g->worms[j].segments[0]);
			if(whitgl_fvec_sqmagnitude(dist) < 12*12)
			{
				g->humans[i].alive = false;
				g->worms[j].has_ripple[0] = true;
				if(j==g->player)
					whitgl_sound_play(whitgl_randint(SOUND_OM3+1), 0.9+whitgl_randfloat()/5);
			}
		}
	}
	if(g->drill.attack.type != DAMAGE_NONE)
	{
		game_do_damage(g, g->drill.attack);
		whitgl_fvec drill_pos = {whitgl_randint(g->land->size.x), 0};
		g->drill = driller_zero(drill_pos);
	}
	whitgl_fcircle splat = whitgl_fcircle_zero;
	for(i=0; i<MAX_WORMS; i++)
	{
		splat.pos = g->worms[i].segments[7]; // can't do 0 because it mucks up land_get
		splat.size = 6;
		land_splat(g->land, splat);
	}
	splat.pos = g->drill.pos;
	splat.size = 2;
	land_splat(g->land, splat);

	for(i=0; i<MAX_DISPLAYED_DAMAGES; i++)
	{
		if(display_damages[i].timer <= 0)
			continue;
		display_damages[i].timer -= 0.2;
	}
}

void _game_add_display_damage(whitgl_fcircle circle)
{
	int i;
	for(i=0; i<MAX_DISPLAYED_DAMAGES; i++)
	{
		if(display_damages[i].timer > 0)
			continue;
		display_damages[i].circle = circle;
		display_damages[i].timer = 1;
		return;
	}
}

void game_do_damage(ld29_game* g, ld29_damage damage)
{
	switch(damage.type)
	{
		case DAMAGE_SIDE:
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
			break;
		}
		case DAMAGE_BLAST:
		{
			whitgl_fcircle splat = whitgl_fcircle_zero;
			splat.pos = damage.pos;
			splat.size = 50;
			land_splat(g->land, splat);
			_game_add_display_damage(splat);
			int i;
			for(i=0; i<MAX_HUMANS; i++)
			{
				whitgl_fvec dist = whitgl_fvec_sub(g->humans[i].pos, splat.pos);
				if(whitgl_fvec_sqmagnitude(dist) < splat.size*splat.size)
					g->humans[i].alive = false;
			}
			int j;
			for(i=0; i<MAX_WORMS; i++)
			{
				for(j=g->worms[i].num_segments-1; j>=0; j--)
				{
					whitgl_fvec dist = whitgl_fvec_sub(g->worms[i].segments[j], splat.pos);
					if(whitgl_fvec_sqmagnitude(dist) < splat.size*splat.size)
						g->worms[i].hurt_segment = j;
				}
			}
			break;
		}
		default: break;
	}

}

void _game_display_damages(whitgl_ivec camera)
{
	int i;
	for(i=0; i<MAX_DISPLAYED_DAMAGES; i++)
	{
		if(display_damages[i].timer < 0)
			continue;
		whitgl_sys_color yellow_color = {0xed, 0xc7, 0x31, 0xff};
		whitgl_sys_color white_color = {0xf4, 0xc2, 0xde, 0xff};
		whitgl_sys_color col = display_damages[i].timer > 0.6 ? white_color : yellow_color;
		whitgl_fcircle draw_circle = display_damages[i].circle;
		draw_circle.pos = whitgl_fvec_add(draw_circle.pos, whitgl_ivec_to_fvec(camera));
		whitgl_sys_draw_fcircle(draw_circle, col, 32);
	}
}

void game_draw(const ld29_game* g, whitgl_ivec screen_size)
{
	whitgl_ivec camera = whitgl_ivec_inverse(whitgl_fvec_to_ivec(g->worms[0].segments[0]));
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

	whitgl_ivec camera_a = camera;
	whitgl_ivec camera_b = camera;
	camera_b.x += g->land->size.x*wrap_dir;

	land_draw(g->land, camera_a);
	land_draw(g->land, camera_b);
	int i;
	for(i=0; i<MAX_HUMANS; i++)
	{
		human_draw(g->humans[i], camera_a);
		human_draw(g->humans[i], camera_b);
	}
	for(i=0; i<MAX_WORMS; i++)
	{
		worm_draw(g->worms[i], camera_a);
		worm_draw(g->worms[i], camera_b);
	}
	egg_draw(g->egg, camera_a);
	egg_draw(g->egg, camera_b);
	driller_draw(g->drill, camera_a);
	driller_draw(g->drill, camera_b);
	_game_display_damages(camera_a);
	_game_display_damages(camera_b);
}