#include "driller.h"

#include <whitgl/logging.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>

#include <image.h>
#include <sounds.h>

ld29_driller driller_zero(whitgl_fvec pos)
{
	ld29_driller out;
	out.pos = pos;
	out.speed = whitgl_fvec_zero;
	out.drill_timer = 0;
	out.beam_charge = 0;
	out.attack = damage_zero;
	out.state = DRILLER_LANDING;
	out.drill_volume = 0;
	return out;
}
ld29_driller driller_update(ld29_driller in, const ld29_land* land)
{
	ld29_driller out = driller_zero(in.pos);
	out.drill_timer = in.drill_timer + 0.05;
	if(out.drill_timer > 1) out.drill_timer--;

	whitgl_ivec offset = {0, 4};

	ld29_land_type land_type = land_get(land, whitgl_ivec_add(whitgl_fvec_to_ivec(in.pos), offset));
	bool in_land = land_type == LAND_GROUND || land_type == LAND_BEDROCK;

	bool entered_space = true;
	int i;
	offset.x = 0; offset.y = 1;
	for(i=0; i<5; i++)
	{
		whitgl_ivec scale = whitgl_ivec_zero;
		scale.x = i; scale.y = i;
		if(land_get(land, whitgl_ivec_add(whitgl_fvec_to_ivec(in.pos), whitgl_ivec_scale(offset, scale))) == LAND_GROUND)
			entered_space = false;
	}

	out.state = in.state;
	if(!in_land)
	{
		out.speed.y = in.speed.y + 0.05;
		if(entered_space && in.state == DRILLER_DRILLING) out.state = DRILLER_PRIMED;
	}
	if(in_land && in.state == DRILLER_LANDING)
	{
		out.state = DRILLER_DRILLING;

	}
	if(in_land && in.beam_charge == 0)
	{
		if(land_type == LAND_BEDROCK)
		{
			out.state = DRILLER_PRIMED;
			out.speed.y = 0.0;
		} else
		{
			out.speed.y = 0.5;
		}
	}
	if(in.state != DRILLER_PRIMED && out.state == DRILLER_PRIMED)
		whitgl_sound_play(SOUND_TRIGGER, 0.95+whitgl_randfloat()*0.1);
	if(in.state == DRILLER_PRIMED)
		out.beam_charge = in.beam_charge + 2.0/60.0;
	if(in_land && out.beam_charge > 1)
	{
		out.beam_charge = 1;
		out.attack.type = DAMAGE_BLAST;
		out.attack.pos = out.pos;
		whitgl_sound_play(SOUND_POP, 0.9+whitgl_randfloat()*0.2);
	}
	out.pos = whitgl_fvec_add(in.pos, out.speed);

	float target_drill_volume = 0;
	if(out.state == DRILLER_DRILLING)
		target_drill_volume = 1;
	out.drill_volume = (target_drill_volume+in.drill_volume*4)/5;
	whitgl_loop_volume(SOUND_DRILL, out.drill_volume);
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
