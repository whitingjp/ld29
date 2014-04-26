#include "land.h"

#include <whitgl/sys.h>


land land_zero()
{
	land out;
	out.size.x = LAND_XRES;
	out.size.y = LAND_YRES;

	int i;
	for(i=0; i<out.size.x*out.size.y*3; i+=3)
	{
		int y = i/(out.size.x*3);
		if(y > 200)
		{
			out.data[i] = 0xb5;
			out.data[i+1] = 0x1d;
			out.data[i+2] = 0x3d;
		}
		else
		{
			out.data[i] = 0x5a;
			out.data[i+1] = 0x1c;
			out.data[i+2] = 0x28;
		}
	}	
	return out;
}
void land_splat(land* land, whitgl_fcircle c)
{
	whitgl_ivec center = whitgl_fvec_to_ivec(c.pos);
	whitgl_iaabb bounds = whitgl_iaabb_zero;
	whitgl_ivec offset = {c.size, c.size};
	bounds.a = whitgl_ivec_sub(center, offset);
	bounds.b = whitgl_ivec_add(center, offset);
	whitgl_ivec pos;
	for(pos.x=bounds.a.x; pos.x<=bounds.b.x; pos.x++)
	{
		for(pos.y=bounds.a.y; pos.y<=bounds.b.y; pos.y++)
		{
			int index = (pos.x+pos.y*land->size.x)*3;
			if(index < 0 || index >= land->size.x*land->size.y*3)
				continue;
			whitgl_float sqmag = whitgl_ivec_sqmagnitude(whitgl_ivec_sub(center, pos));
			if(sqmag > c.size)
				continue;
			if(land->data[index] == 0x5a)
				continue;
			land->data[index] = 0x63;
			land->data[index+1] = 0x28;
			land->data[index+2] = 0x47;
		}
	}	
}

void land_draw(const land* land)
{
	whitgl_sys_image_from_data(4, land->size, land->data);
	whitgl_iaabb src = whitgl_iaabb_zero;
	src.b = land->size;
	whitgl_iaabb dest = src;
	whitgl_sys_draw_tex_iaabb(4, src, dest);
}
