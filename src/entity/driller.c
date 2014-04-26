#include "driller.h"

#include <whitgl/logging.h>
#include <whitgl/sys.h>

#include <image.h>

driller driller_zero(whitgl_fvec pos)
{
	driller out;
	out.pos = pos;
	out.speed = whitgl_fvec_zero;
	out.drill_timer = 0;
	out.beam_charge = 0;
	return out;
}
driller driller_update(driller in, const land* land, egg e)
{
	driller out = driller_zero(in.pos);
	out.drill_timer = in.drill_timer + 0.05;
	if(out.drill_timer > 1) out.drill_timer--;

	whitgl_ivec offset = {0, 4};

	bool filled = land_filled(land, whitgl_ivec_add(whitgl_fvec_to_ivec(in.pos), offset));
	if(!filled)
		out.speed.y = in.speed.y + 0.05;
	else if(out.pos.y < e.pos.y && in.beam_charge == 0)
		out.speed.y = 0.5;
	else
		out.beam_charge = in.beam_charge + 2.0/60.0;
	if(out.beam_charge > 1) out.beam_charge = 1;
	out.pos = whitgl_fvec_add(in.pos, out.speed);
	return out;
}
void driller_draw(driller d)
{
	whitgl_sprite sprite = whitgl_sprite_zero;
	sprite.image = IMAGE_SPRITES;
	sprite.size.x = 8; sprite.size.y = 8;
	sprite.top_left.y = 32;
	whitgl_ivec frame = whitgl_ivec_zero;
	frame.x = d.beam_charge*3.99;
	whitgl_ivec offset = {-4, -8};
	whitgl_ivec draw_pos = whitgl_ivec_add( whitgl_fvec_to_ivec(d.pos), offset);
	whitgl_sys_draw_sprite(sprite, frame, draw_pos);
	offset.y = 0;
	draw_pos = whitgl_ivec_add( whitgl_fvec_to_ivec(d.pos), offset);
	if(d.beam_charge == 0)
		frame.x = d.drill_timer*3;
	else
	 	frame.x = 0;
	frame.y = 1;
	whitgl_sys_draw_sprite(sprite, frame, draw_pos);
}
