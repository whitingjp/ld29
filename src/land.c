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
void land_draw(const land* land)
{
	whitgl_sys_image_from_data(4, land->size, land->data);
	whitgl_iaabb src = whitgl_iaabb_zero;
	src.b = land->size;
	whitgl_iaabb dest = src;
	whitgl_sys_draw_tex_iaabb(4, src, dest);
}
