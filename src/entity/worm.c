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
	return out;
}
ld29_worm worm_update(ld29_worm in, const ld29_land* land)
{
	ld29_worm out;
	int i;
	for(i=0; i<WORM_NUM_SEGMENTS-1; i++)
		out.segments[i+1] = in.segments[i];

	bool filled = land_filled(land, whitgl_fvec_to_ivec(in.segments[0]));
	if(filled)
	{
		whitgl_float dir_speed = 0.2;
		out.dir = in.dir;
		if(whitgl_input_down(WHITGL_INPUT_LEFT))
			out.dir -= dir_speed;
		if(whitgl_input_down(WHITGL_INPUT_RIGHT))
			out.dir += dir_speed;
		whitgl_float speed = 2;
		whitgl_fvec speed_scale = {speed, speed};
		out.speed = whitgl_fvec_scale(whitgl_angle_to_fvec(out.dir), speed_scale);
	} else
	{
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