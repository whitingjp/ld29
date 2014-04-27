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
	out.move_click = 0;
	out.parachuting = 0;
	return out;
}
ld29_human human_update(ld29_human in, const ld29_land* land)
{
	ld29_human out = human_zero(in.pos);
	if(!in.alive)
		return in;

	out.going_left = in.going_left;
	out.move_click = in.move_click;
	out.parachuting = in.parachuting;
	if(out.move_click <= 0)
	{
		int fore_dir = in.going_left ? -1 : 1;

		whitgl_fvec pos_b = in.pos; pos_b.y++;
		ld29_land_type type_b = land_get(land, whitgl_fvec_to_ivec(pos_b));
		whitgl_fvec pos_f = in.pos; pos_f.x += fore_dir;
		ld29_land_type type_f = land_get(land, whitgl_fvec_to_ivec(pos_f));
		whitgl_fvec pos_bf = in.pos; pos_bf.x += fore_dir; pos_bf.y++;
		ld29_land_type type_bf = land_get(land, whitgl_fvec_to_ivec(pos_bf));
		whitgl_fvec pos_tf = in.pos; pos_tf.x += fore_dir; pos_tf.y-=5;
		ld29_land_type type_tf = land_get(land, whitgl_fvec_to_ivec(pos_tf));

		out.pos = in.pos;
		if(type_b == LAND_SKY || type_b == LAND_TUNNEL)
		{
			out.pos.y++; out.parachuting++;
			if((type_bf == LAND_SKY || type_bf == LAND_TUNNEL) && whitgl_randint(2))
				out.pos.x += fore_dir;
		}
		else if(type_tf != LAND_SKY && type_tf != LAND_TUNNEL)
			{ out.pos.x -= fore_dir; out.parachuting = 0; }
		else if(type_f != LAND_SKY && type_f != LAND_TUNNEL)
			{ out.pos.x += fore_dir; out.pos.y--; out.parachuting = 0; }
		else if(type_bf != LAND_SKY && type_bf != LAND_TUNNEL)
			{ out.pos.x += fore_dir; out.pos.y++ ; out.parachuting = 0; }
		else
			{ out.pos.x += fore_dir; out.parachuting = 0; }

		if(whitgl_randfloat() > 0.995)
			out.going_left = !in.going_left;
		out.move_click = 2;
	} else
	{
		out.move_click--;
	}
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
	bool show_parachute = human.parachuting > 20;
	frame.x = show_parachute ? 3 : 2*human.anim;
	frame.y = human.going_left ? 1 : 0;
	whitgl_ivec offset = {-4, -8};
	whitgl_ivec draw_pos = whitgl_ivec_add( whitgl_fvec_to_ivec(human.pos), offset);
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	whitgl_sys_draw_sprite(sprite, frame, draw_pos);
	if(show_parachute)
	{
		int fore_dir = human.going_left ? -1 : 1;
		frame.x = 2;
		offset.x = -4-fore_dir*8; offset.y = -16;
		draw_pos = whitgl_ivec_add( whitgl_fvec_to_ivec(human.pos), offset);
		draw_pos = whitgl_ivec_add(draw_pos, camera);
		whitgl_sys_draw_sprite(sprite, frame, draw_pos);
	}
}