#include "egg.h"

#include <whitgl/sys.h>

egg egg_zero(const land* land)
{
	egg out;
	whitgl_fvec start = whitgl_fvec_zero;
	start.x = land->size.x/2-50;
	start.y = land->size.y-150;
	out.pos = start;
	out.speed = whitgl_fvec_zero;
	return out;
}
egg egg_update(egg in, const land* land)
{
	egg out;
	bool filled = land_filled(land, whitgl_fvec_to_ivec(in.pos));
	out.speed = in.speed;
	if(filled)
	{
		out.speed.y = 0;
		out.pos = in.pos;
	} else
	{
		out.speed.y+=0.05;
		out.pos = whitgl_fvec_add(in.pos, out.speed);
	}
	return out;
}
void egg_draw(egg e)
{
	whitgl_sys_color color = {0xf4, 0xc2, 0xde, 0xff};
	whitgl_fcircle c = whitgl_fcircle_zero;
	c.pos = e.pos;
	c.size = 10;
	whitgl_sys_draw_fcircle(c, color, 16);
}