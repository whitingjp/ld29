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
	out.speed = whitgl_fvec_zero;
	return out;
}
worm worm_update(worm in)
{
	return in;
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