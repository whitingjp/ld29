#include "land.h"

#include <whitgl/logging.h>
#include <whitgl/sys.h>

#include <image.h>

static const whitgl_sys_color land_colors[LAND_MAX] =
{
	{0x5a, 0x1c, 0x28, 0xff},
	{0xb5, 0x1d, 0x3d, 0xff},
	{0x63, 0x28, 0x47, 0xff},
	{0x41, 0x45, 0x4d, 0xff},
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
	if(pos.y >= land->size.y) return LAND_BEDROCK;
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

	whitgl_ivec p;
	for(p.x=0; p.x<land->size.x; p.x++)
	{
		int land_start = 100+whitgl_randint(2);
		int bedrock_start = land->size.y-10+whitgl_randint(2);
		for(p.y=0; p.y<land->size.y; p.y++)
		{
			if(p.y > bedrock_start)
				land_set(land, LAND_BEDROCK, p);
			else if(p.y > land_start)
				land_set(land, LAND_GROUND, p);
			else
				land_set(land, LAND_SKY, p);
		}
	}
	int i;
	for(i=0; i<1024; i++)
		land_update(land);
}
void land_update(ld29_land* land)
{
	int num_checks = 1024*2;
	while(num_checks)
	{
		whitgl_ivec pos = whitgl_ivec_zero;
		pos.x = whitgl_randint(land->size.x);
		pos.y = whitgl_randint(land->size.y);
		whitgl_ivec under_pos = pos;
		under_pos.x += whitgl_randint(3)-1;
		under_pos.y++;
		ld29_land_type type = land_get(land, pos);
		if(type == LAND_BEDROCK || type == LAND_SKY)
			continue;
		ld29_land_type under_type = land_get(land, under_pos);
		if(under_type == LAND_GROUND || under_type == LAND_BEDROCK)
		{
			land_set(land, LAND_GROUND, pos);
		} else if(whitgl_randint(4) == 1)
		{
			if(type == LAND_GROUND)
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
	for(pos.x=bounds.a.x; pos.x<=bounds.b.x; pos.x++)
	{
		for(pos.y=bounds.a.y; pos.y<=bounds.b.y; pos.y++)
		{
			whitgl_int sqmag = whitgl_ivec_sqmagnitude(whitgl_ivec_sub(center, pos));
			if(sqmag > c.size*c.size)
				continue;
			ld29_land_type type = land_get(land, pos);
			if(type == LAND_SKY || type == LAND_BEDROCK)
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
