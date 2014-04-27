#include "human.h"

#include <whitgl/logging.h>
#include <whitgl/sys.h>

#include <image.h>

ld29_human human_zero(whitgl_fvec pos)
{
	ld29_human out;
	out.alive = true;
	out.pos = pos;
	out.going_left = whitgl_randfloat()>0.5;
	out.anim = whitgl_randfloat();
	return out;
}
ld29_human human_update(ld29_human in, const ld29_land* land)
{
	ld29_human out = human_zero(in.pos);
	if(!in.alive)
		return in;
	// int xdir = in.going

	out.pos = in.pos;
	whitgl_fvec pos_b = in.pos; pos_b.y++;
	ld29_land_type type_b = land_get(land, whitgl_fvec_to_ivec(pos_b));
	if(type_b == LAND_SKY || type_b == LAND_TUNNEL)
		out.pos.y++;
	if(whitgl_randfloat() > 0.995)
		out.going_left = !in.going_left;
	else
		out.going_left = in.going_left;
	out.anim = whitgl_fwrap(in.anim+0.025, 0, 1);
	return out;
}
void human_draw(ld29_human human, whitgl_ivec camera)
{
	if(!human.alive)
		return;
	whitgl_sprite sprite = whitgl_sprite_zero;
	sprite.image = IMAGE_SPRITES;
	sprite.size.x = 8; sprite.size.y = 8;
	sprite.top_left.x = 32; sprite.top_left.y = 16;
	whitgl_ivec frame = whitgl_ivec_zero;
	frame.x = 2*human.anim;
	whitgl_ivec offset = {-4, -8};
	whitgl_ivec draw_pos = whitgl_ivec_add( whitgl_fvec_to_ivec(human.pos), offset);
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	whitgl_sys_draw_sprite(sprite, frame, draw_pos);
}