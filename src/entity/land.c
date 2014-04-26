#include "land.h"

#include <whitgl/sys.h>

#include <image.h>

void land_zero(ld29_land* l)
{
	l->size.x = LAND_XRES;
	l->size.y = LAND_YRES;

	int i;
	for(i=0; i<l->size.x*l->size.y*3; i+=3)
	{
		int y = i/(l->size.x*3);
		if(y > 100)
		{
			l->data[i] = 0xb5;
			l->data[i+1] = 0x1d;
			l->data[i+2] = 0x3d;
		}
		else
		{
			l->data[i] = 0x5a;
			l->data[i+1] = 0x1c;
			l->data[i+2] = 0x28;
		}
	}
}
void land_update(ld29_land* land)
{
	int num_checks = 1024;
	while(num_checks)
	{
		whitgl_ivec pos = whitgl_ivec_zero;
		pos.x = whitgl_randint(land->size.x);
		pos.y = whitgl_randint(land->size.y-1);
		whitgl_ivec under_pos = pos;
		under_pos.x += whitgl_randint(3)-1;
		under_pos.y++;
		if(land_filled(land, under_pos))
		{
			int index = (pos.x+pos.y*land->size.x)*3;
			if(land->data[index] == 0x63)
			{
				land->data[index] = 0xb5;
				land->data[index+1] = 0x1d;
				land->data[index+2] = 0x3d;
			}
		} else if(whitgl_randint(4) == 1)
		{
			int index = (pos.x+pos.y*land->size.x)*3;
			if(land->data[index] == 0xb5)
			{
				land->data[index] = 0x63;
				land->data[index+1] = 0x28;
				land->data[index+2] = 0x47;
			}
		}
		num_checks--;
	}
}
void land_splat(ld29_land* land, whitgl_fcircle c)
{
	whitgl_ivec center = whitgl_fvec_to_ivec(c.pos);
	whitgl_iaabb bounds = whitgl_iaabb_zero;
	whitgl_ivec offset = {c.size, c.size};
	bounds.a = whitgl_ivec_sub(center, offset);
	bounds.b = whitgl_ivec_add(center, offset);
	whitgl_ivec pos;
	whitgl_iaabb land_bounds = whitgl_iaabb_zero;
	land_bounds.b = land->size;
	bounds = whitgl_iaabb_intersection(bounds, land_bounds);
	for(pos.x=bounds.a.x; pos.x<=bounds.b.x; pos.x++)
	{
		for(pos.y=bounds.a.y; pos.y<=bounds.b.y; pos.y++)
		{
			int index = (pos.x+pos.y*land->size.x)*3;
			if(index < 0 || index >= land->size.x*land->size.y*3)
				continue;
			whitgl_int sqmag = whitgl_ivec_sqmagnitude(whitgl_ivec_sub(center, pos));
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

void land_draw(const ld29_land* land)
{
	whitgl_sys_image_from_data(IMAGE_LAND, land->size, land->data);
	whitgl_iaabb src = whitgl_iaabb_zero;
	src.b = land->size;
	whitgl_iaabb dest = src;
	whitgl_sys_draw_tex_iaabb(IMAGE_LAND, src, dest);
}

bool land_filled(const ld29_land* land, whitgl_ivec p)
{
	if(p.x < 0 || p.x >= land->size.x)
		return true;
	if(p.y < 0 || p.y >= land->size.y)
		return true;
	int index = (p.x+p.y*land->size.x)*3;
	return land->data[index] == 0xb5;
}
