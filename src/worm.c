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
	out.segments[0] = in.segments[0];
	out.segments[0].y += 0.5;
	out.dir = in.dir;
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