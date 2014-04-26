#include "worm.h"

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/sys.h>

worm worm_zero()
{
	worm out;
	int i;
	for(i=0; i<WORM_NUM_SEGMENTS; i++)
		out.segments[i] = whitgl_fvec_zero;
	out.dir = 0;
	return out;
}
worm worm_update(worm in)
{
	worm out;
	int i;
	for(i=0; i<WORM_NUM_SEGMENTS-1; i++)
		out.segments[i+1] = in.segments[i];

	whitgl_float dir_speed = 0.1;
	out.dir = in.dir;
	if(whitgl_input_down(WHITGL_INPUT_LEFT))
		out.dir -= dir_speed;
	if(whitgl_input_down(WHITGL_INPUT_RIGHT))
		out.dir += dir_speed;

	whitgl_fvec dir_vec = whitgl_angle_to_fvec(out.dir);
	out.segments[0] = whitgl_fvec_add(in.segments[0], dir_vec);	
	return out;
}
void worm_draw(worm w)
{
	whitgl_sys_color color = {0xf4, 0xc2, 0xde, 0xff};
	int i;
	for(i=0; i<WORM_NUM_SEGMENTS; i++)
	{
		whitgl_iaabb rect = {{0,0},{2,2}};
		rect = whitgl_iaabb_add(rect, whitgl_fvec_to_ivec(w.segments[i]));
		whitgl_sys_draw_iaabb(rect, color);
	}
}