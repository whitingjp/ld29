#include "worm.h"

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/sys.h>

ld29_worm worm_zero(const ld29_land* land)
{
	ld29_worm out;
	whitgl_fvec start = whitgl_fvec_zero;
	start.x = land->size.x/2+50;
	start.y = land->size.y;
	int i;
	for(i=0; i<WORM_NUM_SEGMENTS; i++)
		out.segments[i] = start;
	out.speed = whitgl_fvec_zero;
	out.dir = -whitgl_pi/2;
	out.boost = 0;
	out.boost_dir = 0;
	return out;
}
ld29_worm worm_update(ld29_worm in, const ld29_land* land)
{
	ld29_worm out = worm_zero(land);
	int i;
	for(i=0; i<WORM_NUM_SEGMENTS-1; i++)
		out.segments[i+1] = in.segments[i];

	bool filled = land_filled(land, whitgl_fvec_to_ivec(in.segments[0]));
	if(filled)
	{
		whitgl_float dir_speed = 0.15;
		whitgl_float boost_dir_speed = 0.03;
		out.dir = in.dir;
		out.boost_dir = in.boost_dir;
		out.boost = in.boost;
		bool boosting = false;
		if(whitgl_input_down(WHITGL_INPUT_LEFT))
		{
			out.dir -= dir_speed;
			out.boost_dir = whitgl_fmax(-1, in.boost_dir-boost_dir_speed);
			boosting = out.boost_dir > -0.5;
		}
		if(whitgl_input_down(WHITGL_INPUT_RIGHT))
		{
			out.dir += dir_speed;
			out.boost_dir = whitgl_fmin(1, in.boost_dir+boost_dir_speed);
			boosting = out.boost_dir < 0.5;
		}
		whitgl_float boost_inc = 0.03;
		if(boosting)
		{
			out.boost = whitgl_fmin(10, out.boost+boost_inc);
		}
		else
		{
			out.boost = whitgl_fmax(0, out.boost-boost_inc*(1+out.boost));
			out.boost_dir = out.boost_dir*0.9;
		}

		whitgl_float speed = 1.5*(1+whitgl_sqrt(whitgl_sqrt(out.boost)));
		// WHITGL_LOG("boost_dir %.2f boost %.2f speed %.2f", out.boost_dir, out.boost, speed);
		whitgl_fvec speed_scale = {speed, speed};
		out.speed = whitgl_fvec_scale(whitgl_angle_to_fvec(out.dir), speed_scale);
	} else
	{
		out.boost = in.boost;
		out.boost_dir = in.boost_dir;
		out.speed.x = in.speed.x;
		out.speed.y = in.speed.y+0.05;
		out.dir = whitgl_fvec_to_angle(in.speed);
	}

	out.segments[0] = whitgl_fvec_add(in.segments[0], out.speed);
	return out;
}
void worm_draw(ld29_worm worm, whitgl_ivec camera)
{
	whitgl_sys_color color = {0xf4, 0xc2, 0xde, 0xff};
	int i;
	for(i=0; i<WORM_NUM_SEGMENTS; i++)
	{
		whitgl_fcircle c = whitgl_fcircle_zero;
		c.pos = whitgl_fvec_add(worm.segments[i], whitgl_ivec_to_fvec(camera));
		c.size = (whitgl_float)(WORM_NUM_SEGMENTS-i+12)/12;
		if(i%4==0) c.size *= 1.25;
		whitgl_sys_draw_fcircle(c, color, 16);
	}
}