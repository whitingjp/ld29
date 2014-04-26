#include "driller.h"

#include <whitgl/logging.h>
#include <whitgl/sys.h>

#include <image.h>

ld29_driller driller_zero(whitgl_fvec pos)
{
	ld29_driller out;
	out.pos = pos;
	out.speed = whitgl_fvec_zero;
	out.drill_timer = 0;
	out.beam_charge = 0;
	out.attack = damage_zero;
	return out;
}
ld29_driller driller_update(ld29_driller in, const ld29_land* land, ld29_egg e)
{
	ld29_driller out = driller_zero(in.pos);
	out.drill_timer = in.drill_timer + 0.05;
	if(out.drill_timer > 1) out.drill_timer--;

	whitgl_ivec offset = {0, 4};

	bool filled = land_filled(land, whitgl_ivec_add(whitgl_fvec_to_ivec(in.pos), offset));
	if(!filled)
		out.speed.y = in.speed.y + 0.05;
	else if(e.dead || (out.pos.y < e.pos.y && in.beam_charge == 0))
		out.speed.y = 0.5;
	else
		out.beam_charge = in.beam_charge + 2.0/60.0;
	if(out.beam_charge > 1)
	{
		out.beam_charge = 1;
		out.attack.type = DAMAGE_SIDE;
		out.attack.pos = out.pos;
	}
	out.pos = whitgl_fvec_add(in.pos, out.speed);
	return out;
}
void driller_draw(ld29_driller driller, whitgl_ivec camera)
{
	whitgl_sprite sprite = whitgl_sprite_zero;
	sprite.image = IMAGE_SPRITES;
	sprite.size.x = 8; sprite.size.y = 8;
	sprite.top_left.y = 32;
	whitgl_ivec frame = whitgl_ivec_zero;
	frame.x = driller.beam_charge*3.99;
	whitgl_ivec offset = {-4, -8};
	whitgl_ivec draw_pos = whitgl_ivec_add( whitgl_fvec_to_ivec(driller.pos), offset);
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	whitgl_sys_draw_sprite(sprite, frame, draw_pos);
	offset.y = 0;
	draw_pos = whitgl_ivec_add( whitgl_fvec_to_ivec(driller.pos), offset);
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	if(driller.beam_charge == 0)
		frame.x = driller.drill_timer*3;
	else
	 	frame.x = 0;
	frame.y = 1;
	whitgl_sys_draw_sprite(sprite, frame, draw_pos);
}
