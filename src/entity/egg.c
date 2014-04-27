#include "egg.h"

#include <whitgl/sys.h>

#include <image.h>

ld29_egg egg_zero()
{
	ld29_egg out;
	out.pos = whitgl_fvec_zero;
	out.speed = whitgl_fvec_zero;
	out.dead = true;
	out.hatch = 0;
	out.bounce = 0;
	return out;
}
ld29_egg egg_update(ld29_egg in, const ld29_land* land)
{
	if(in.dead)
		return in;
	ld29_egg out = egg_zero(land);
	out.hatch = in.hatch;
	out.dead = false;
	out.speed = in.speed;
	out.bounce = in.bounce;
	ld29_land_type land_type = land_get(land, whitgl_fvec_to_ivec(in.pos));
	if(land_type == LAND_GROUND || land_type == LAND_BEDROCK)
	{
		out.speed.y = 0;
		out.pos = in.pos;
	} else
	{
		out.speed.y+=0.05;
		out.pos = whitgl_fvec_add(in.pos, out.speed);
	}
	out.hatch += 1.0/(60.0*10);
	if(out.bounce == 0 && whitgl_randfloat() > 0.95)
		out.bounce = 1;
	out.bounce = whitgl_fmax(0, out.bounce-0.05);
	return out;
}
void egg_draw(ld29_egg egg, whitgl_ivec camera)
{
	if(egg.dead)
		return;
	whitgl_sprite sprite = whitgl_sprite_zero;
	sprite.image = IMAGE_SPRITES;
	sprite.size.x = 16; sprite.size.y = 16;
	whitgl_ivec frame = whitgl_ivec_zero;
	float bounce_y = whitgl_fsin(egg.bounce*whitgl_pi)*10*(0.2+egg.hatch*0.8);
	whitgl_ivec offset = {-8, -8-bounce_y};
	whitgl_ivec draw_pos = whitgl_ivec_add( whitgl_fvec_to_ivec(egg.pos), offset);
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	whitgl_sys_draw_sprite(sprite, frame, draw_pos);
}