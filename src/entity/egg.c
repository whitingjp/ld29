#include "egg.h"

#include <whitgl/sys.h>

#include <image.h>

ld29_egg egg_zero()
{
	ld29_egg out;
	out.pos = whitgl_fvec_zero;
	out.speed = whitgl_fvec_zero;
	out.dead = true;
	return out;
}
ld29_egg egg_update(ld29_egg in, const ld29_land* land)
{
	if(in.dead)
		return in;
	ld29_egg out = egg_zero(land);
	out.dead = false;
	out.speed = in.speed;
	if(land_get(land, whitgl_fvec_to_ivec(in.pos)) == LAND_GROUND)
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
	sprite.size.x = 16; sprite.size.y = 16;
	whitgl_ivec frame = whitgl_ivec_zero;
	whitgl_ivec offset = {-8, -8};
	whitgl_ivec draw_pos = whitgl_ivec_add( whitgl_fvec_to_ivec(egg.pos), offset);
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	whitgl_sys_draw_sprite(sprite, frame, draw_pos);
}