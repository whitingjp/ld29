#include "egg.h"

#include <whitgl/sys.h>

#include <image.h>

ld29_egg egg_zero(const ld29_land* land)
{
	ld29_egg out;
	whitgl_fvec start = whitgl_fvec_zero;
	start.x = land->size.x/2-50;
	start.y = land->size.y-150;
	out.pos = start;
	out.speed = whitgl_fvec_zero;
	out.dead = false;
	return out;
}
ld29_egg egg_update(ld29_egg in, const ld29_land* land)
{
	if(in.dead)
		return in;
	ld29_egg out = egg_zero(land);
	bool filled = land_filled(land, whitgl_fvec_to_ivec(in.pos));
	out.speed = in.speed;
	if(filled)
	{
		out.speed.y = 0;
		out.pos = in.pos;
	} else
	{
		out.speed.y+=0.05;
		out.pos = whitgl_fvec_add(in.pos, out.speed);
	}
	return out;
}
void egg_draw(ld29_egg egg, whitgl_ivec camera)
{
	if(egg.dead)
		return;
	whitgl_sprite sprite = whitgl_sprite_zero;
	sprite.image = IMAGE_SPRITES;
	sprite.size.x = 32; sprite.size.y = 32;
	whitgl_ivec frame = whitgl_ivec_zero;
	whitgl_ivec offset = {-16, -16};
	whitgl_ivec draw_pos = whitgl_ivec_add( whitgl_fvec_to_ivec(egg.pos), offset);
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	whitgl_sys_draw_sprite(sprite, frame, draw_pos);
}