#include "worm.h"

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/sys.h>

ld29_worm worm_zero(const ld29_land* land)
{
	ld29_worm out;
	whitgl_fvec start = whitgl_fvec_zero;
	start.x = land->size.x/2+50;
	start.y = land->size.y-50;
	int i;
	for(i=0; i<WORM_NUM_SEGMENTS; i++)
		out.segments[i] = start;
	out.speed = whitgl_fvec_zero;
	out.dir = -whitgl_pi/2;
	out.boost = 0;
	out.boost_dir = 0;
	out.maw_anim = 0;
	out.ripple = -1;
	return out;
}
ld29_worm worm_update(ld29_worm in, const ld29_land* land)
{
	ld29_worm out = worm_zero(land);
	int i;
	for(i=0; i<WORM_NUM_SEGMENTS-1; i++)
		out.segments[i+1] = in.segments[i];

	ld29_land_type type = land_get(land, whitgl_fvec_to_ivec(in.segments[0]));
	bool in_land = type == LAND_GROUND;
	if(type == LAND_BEDROCK)
	{
		if(in.speed.x*in.speed.x < 0.2)
			out.speed.x = whitgl_randfloat();
		else
			out.speed.x = in.speed.x;
		out.speed.y = -in.speed.y*1.5;
		out.dir = whitgl_fvec_to_angle(out.speed);
		while(type == LAND_BEDROCK)
		{
			in.segments[0].y--;
			type = land_get(land, whitgl_fvec_to_ivec(in.segments[0]));
		}
	} else if(in_land)
	{
		whitgl_float dir_speed = 0.15;
		whitgl_float boost_dir_speed = 0.03;
		out.dir = in.dir;
		out.boost_dir = in.boost_dir;
		out.boost = in.boost;
		bool boosting = false;
		if(whitgl_input_down(WHITGL_INPUT_LEFT))
		{
			out.dir -= dir_speed;
			out.boost_dir = whitgl_fmax(-1, in.boost_dir-boost_dir_speed);
			boosting = out.boost_dir > -0.5;
		}
		if(whitgl_input_down(WHITGL_INPUT_RIGHT))
		{
			out.dir += dir_speed;
			out.boost_dir = whitgl_fmin(1, in.boost_dir+boost_dir_speed);
			boosting = out.boost_dir < 0.5;
		}
		whitgl_float boost_inc = 0.03;
		if(boosting)
		{
			out.boost = whitgl_fmin(10, out.boost+boost_inc);
		}
		else
		{
			out.boost = whitgl_fmax(0, out.boost-boost_inc*(1+out.boost));
			out.boost_dir = out.boost_dir*0.9;
		}

		whitgl_float speed = 1.5*(1+whitgl_fsqrt(whitgl_fsqrt(out.boost)));
		// WHITGL_LOG("boost_dir %.2f boost %.2f speed %.2f", out.boost_dir, out.boost, speed);
		whitgl_fvec speed_scale = {speed, speed};
		out.speed = whitgl_fvec_scale(whitgl_angle_to_fvec(out.dir), speed_scale);
	} else
	{
		out.boost = in.boost;
		out.boost_dir = in.boost_dir;
		out.speed.x = in.speed.x*0.995;
		out.speed.y = in.speed.y+0.07;
		out.dir = whitgl_fvec_to_angle(in.speed);
	}

	out.segments[0] = whitgl_fvec_add(in.segments[0], out.speed);
	out.segments[0].x = whitgl_fwrap(out.segments[0].x, 0, land->size.x);

	float maw_speed = whitgl_fsqrt(whitgl_fvec_sqmagnitude(out.speed))/35;
	if(in_land || in.maw_anim != 0)
		out.maw_anim = in.maw_anim+maw_speed;
	else
		out.maw_anim = in.maw_anim;
	if(out.maw_anim > 1)
	{
		out.maw_anim--;
		if(!in_land) out.maw_anim = 0;
	}

	out.ripple = in.ripple;
	if(out.ripple != -1)
	{
		out.ripple++;
		if(out.ripple >= WORM_NUM_SEGMENTS)
			out.ripple = -1;
	}
	return out;
}
void worm_draw(ld29_worm worm, whitgl_ivec camera)
{
	whitgl_sys_color color = {0xf4, 0xc2, 0xde, 0xff};
	int i;
	for(i=0; i<WORM_NUM_SEGMENTS; i++)
	{
		whitgl_fcircle c = whitgl_fcircle_zero;
		c.pos = whitgl_fvec_add(worm.segments[i], whitgl_ivec_to_fvec(camera));
		c.size = (whitgl_float)(WORM_NUM_SEGMENTS-i+12)/12;
		if(i%4==0) c.size *= 1.25;
		if(i==worm.ripple) c.size *= 1.5;
		whitgl_sys_draw_fcircle(c, color, 16);
	}
	// maw
	whitgl_fvec maw;
	whitgl_float maw_ang_off = (whitgl_pi/5)+whitgl_fcos(worm.maw_anim*whitgl_pi*2)/5;
	whitgl_fvec maw_off_scale;
	whitgl_fvec maw_off;
	whitgl_fcircle mawc = whitgl_fcircle_zero;

	for(i=0; i<2; i++)
	{
		maw_off = whitgl_angle_to_fvec(worm.dir-maw_ang_off);
		maw_off_scale.x = 9; maw_off_scale.y = 9;
		maw = whitgl_fvec_add(worm.segments[0], whitgl_fvec_scale(maw_off, maw_off_scale));
		mawc.pos = whitgl_fvec_add(maw, whitgl_ivec_to_fvec(camera));
		mawc.size = 1.5;
		whitgl_sys_draw_fcircle(mawc, color, 16);

		maw_off = whitgl_angle_to_fvec(worm.dir-maw_ang_off*1.2);
		maw_off_scale.x = 6; maw_off_scale.y = 6;
		maw = whitgl_fvec_add(worm.segments[0], whitgl_fvec_scale(maw_off, maw_off_scale));
		mawc.pos = whitgl_fvec_add(maw, whitgl_ivec_to_fvec(camera));
		mawc.size = 2.5;
		whitgl_sys_draw_fcircle(mawc, color, 16);

		maw_ang_off = -maw_ang_off;
	}


}