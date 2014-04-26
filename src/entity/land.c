#include "land.h"

#include <whitgl/logging.h>
#include <whitgl/sys.h>

#include <image.h>

static const whitgl_sys_color land_colors[LAND_MAX] =
{
	{0x5a, 0x1c, 0x28, 0xff},
	{0xb5, 0x1d, 0x3d, 0xff},
	{0x63, 0x28, 0x47, 0xff},
};

void land_set(ld29_land* land, ld29_land_type t, whitgl_ivec pos)
{
	if(pos.y < 0 || pos.y >= land->size.y) return;
	pos.x = whitgl_iwrap(pos.x, 0, land->size.x);
	int index = (pos.x+pos.y*land->size.x)*3;
	land->data[index] = land_colors[t].r;
	land->data[index+1] = land_colors[t].g;
	land->data[index+2] = land_colors[t].b;
}

ld29_land_type land_get(const ld29_land* land, whitgl_ivec pos)
{
	if(pos.y < 0) return LAND_SKY;
	if(pos.y >= land->size.y) return LAND_GROUND;
	pos.x = whitgl_iwrap(pos.x, 0, land->size.x);
	int index = (pos.x+pos.y*land->size.x)*3;
	int i;
	for(i=0; i<LAND_MAX; i++)
	{
		if(land->data[index] == land_colors[i].r)
			return i;
	}
	WHITGL_LOG("land_get unrecognised land type %d %d %d", land_colors[i].r, land_colors[i].g, land_colors[i].b);
	return LAND_SKY;
}

void land_zero(ld29_land* land)
{
	land->size.x = LAND_XRES;
	land->size.y = LAND_YRES;

	int i;
	for(i=0; i<land->size.x*land->size.y*3; i+=3)
	{
		int y = i/(land->size.x*3);
		if(y > 100)
		{
			land->data[i] = 0xb5;
			land->data[i+1] = 0x1d;
			land->data[i+2] = 0x3d;
		}
		else
		{
			land->data[i] = 0x5a;
			land->data[i+1] = 0x1c;
			land->data[i+2] = 0x28;
		}
	}
	land_erode(land);
}
void land_erode(ld29_land* land)
{
	int num_checks = 1024*1024*5;
	while(num_checks)
	{
		whitgl_ivec pos = whitgl_ivec_zero;
		pos.x = whitgl_randint(land->size.x);
		pos.y = whitgl_randint(land->size.y);
		whitgl_ivec test_pos = pos;
		test_pos.x += whitgl_randint(3)-1;
		test_pos.y += whitgl_randint(3)-1;
		if(land_get(land, test_pos) == LAND_GROUND)
		{
			land_set(land, LAND_GROUND, pos);
		} else
		{
			if(land_get(land, pos) == LAND_GROUND)
				land_set(land, LAND_TUNNEL, pos);
		}
		num_checks--;
	}
}
void land_update(ld29_land* land)
{
	int num_checks = 1024;
	while(num_checks)
	{
		whitgl_ivec pos = whitgl_ivec_zero;
		pos.x = whitgl_randint(land->size.x);
		pos.y = whitgl_randint(land->size.y);
		whitgl_ivec under_pos = pos;
		under_pos.x += whitgl_randint(3)-1;
		under_pos.y++;
		if(land_get(land, under_pos) == LAND_GROUND)
		{
			if(land_get(land, pos) != LAND_SKY)
			land_set(land, LAND_GROUND, pos);
		} else if(whitgl_randint(4) == 1)
		{
			if(land_get(land, pos) == LAND_GROUND)
				land_set(land, LAND_TUNNEL, pos);
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
			whitgl_int sqmag = whitgl_ivec_sqmagnitude(whitgl_ivec_sub(center, pos));
			if(sqmag > c.size*c.size)
				continue;
			if(land_get(land, pos) == LAND_SKY)
				continue;
			land_set(land, LAND_TUNNEL, pos);
		}
	}
}

void land_draw(const ld29_land* land, whitgl_ivec camera)
{
	whitgl_sys_image_from_data(IMAGE_LAND, land->size, land->data);
	whitgl_iaabb src = whitgl_iaabb_zero;
	src.b = land->size;
	whitgl_iaabb dest = whitgl_iaabb_zero;
	dest.a = camera;
	dest.b = whitgl_ivec_add(camera, land->size);
	whitgl_sys_draw_tex_iaabb(IMAGE_LAND, src, dest);
}
