#include "egg.h"

#include <whitgl/sys.h>

#include <image.h>

egg egg_zero(const land* land)
{
	egg out;
	whitgl_fvec start = whitgl_fvec_zero;
	start.x = land->size.x/2-50;
	start.y = land->size.y-150;
	out.pos = start;
	out.speed = whitgl_fvec_zero;
	return out;
}
egg egg_update(egg in, const land* land)
{
	egg out;
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
void egg_draw(egg e)
{
	whitgl_sprite sprite = whitgl_sprite_zero;
	sprite.image = IMAGE_SPRITES;
	sprite.size.x = 32; sprite.size.y = 32;
	whitgl_ivec frame = whitgl_ivec_zero;
	whitgl_ivec offset = {-16, -16};
	whitgl_ivec draw_pos = whitgl_ivec_add( whitgl_fvec_to_ivec(e.pos), offset);
	whitgl_sys_draw_sprite(sprite, frame, draw_pos);
}