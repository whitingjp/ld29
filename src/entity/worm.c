#include "worm.h"

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>
#include <sounds.h>

ld29_worm worm_zero(const ld29_land* land)
{
	ld29_worm out;
	whitgl_fvec start = whitgl_fvec_zero;
	start.x = whitgl_randint(land->size.x);
	start.y = land->size.y-50;
	int i;
	for(i=0; i<WORM_MAX_SEGMENTS; i++)
	{
		out.segments[i] = start;
		out.has_ripple[i] = false;
	}
	out.speed = whitgl_fvec_zero;
	out.dir = whitgl_pi/2;
	out.boost = 0;
	out.boost_dir = 0;
	out.maw_anim = 0;
	out.vol_current = ld29_worm_volumes_zero;
	out.vol_target = ld29_worm_volumes_zero;
	out.air_time = 0;
	out.num_segments = WORM_MIN_SEGMENTS*2;
	// out.num_segments = WORM_MAX_SEGMENTS;
	out.hurt_segment = -1;
	out.pregnancy = 0;
	out.alive = false;
	out.ai = ld29_worm_ai_zero;
	return out;
}
ld29_worm worm_update(ld29_worm in, const ld29_land* land, bool is_player)
{
	ld29_worm out = worm_zero(land);
	if(!in.alive)
		return out;
	out.alive = in.alive;
	out.vol_target = in.vol_target;
	out.air_time = in.air_time;
	out.num_segments = in.num_segments;
	out.hurt_segment = in.hurt_segment;
	out.pregnancy = in.pregnancy;
	out.ai = worm_ai_update(in.ai, &in, land);
	int i;
	for(i=0; i<in.num_segments-1; i++)
		out.segments[i+1] = in.segments[i];

	if(out.hurt_segment != -1)
	{
		out = in;
		if(out.num_segments <= WORM_MIN_SEGMENTS || out.num_segments <= out.hurt_segment)
			out.hurt_segment = -1;
		else
			out.num_segments--;
		return out;
	}

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
		bool press_left = false;
		bool press_right = false;
		if(is_player)
		{
			press_left = whitgl_input_down(WHITGL_INPUT_LEFT);
			press_right = whitgl_input_down(WHITGL_INPUT_RIGHT);
		} else
		{
			press_left = in.ai.press_left;
			press_right = !in.ai.press_left;
		}
		if(press_left)
		{
			out.dir -= dir_speed;
			out.boost_dir = whitgl_fmax(-1, in.boost_dir-boost_dir_speed);
			boosting = out.boost_dir > -0.5;
			out.vol_target.shhh = boosting ? 0.1 : 0.0;
		} else
		{
			out.vol_target.shhh = 0;
		}
		if(press_right)
		{
			out.dir += dir_speed;
			out.boost_dir = whitgl_fmin(1, in.boost_dir+boost_dir_speed);
			boosting = out.boost_dir < 0.5;
			out.vol_target.shhh = 0.0;
			out.vol_target.shaaa = boosting ? 0.1 : 0.0;
		} else
		{
			out.vol_target.shaaa = 0;
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
		speed *= 0.6+(1-out.pregnancy)*0.4;
		whitgl_fvec speed_scale = {speed, speed};
		out.speed = whitgl_fvec_scale(whitgl_angle_to_fvec(out.dir), speed_scale);

		out.air_time = 0;
		out.vol_target.ground = 0.04*speed;
		out.vol_target.emerge = 0.0;
	} else
	{
		out.air_time++;
		out.vol_target.ground = 0.0;
		if(out.air_time < in.num_segments)
			out.vol_target.emerge = 1.0;
		else
			out.vol_target.emerge = 0.0;
		out.vol_target.shhh = 0.0;
		out.vol_target.shaaa = 0.0;
		out.boost = in.boost;
		out.boost_dir = in.boost_dir;
		out.speed.x = in.speed.x*0.995;
		out.speed.y = in.speed.y+0.07;
		out.dir = whitgl_fvec_to_angle(in.speed);
	}

	if(out.num_segments == WORM_MAX_SEGMENTS || out.pregnancy > 0)
	{
		out.pregnancy += 1.0/(60.0*15);
		int steps = WORM_MAX_SEGMENTS-WORM_MIN_SEGMENTS;
		if((int)(in.pregnancy*steps) != (int)(out.pregnancy*steps))
			out.num_segments--;
		if(out.pregnancy >= 1)
		{
			out.pregnancy = -1;
			whitgl_sound_play(SOUND_ITS_AN_EGG, 1);
		}
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

	for(i=0; i<in.num_segments-1; i++)
		out.has_ripple[i+1] = in.has_ripple[i];
	if(out.has_ripple[in.num_segments-1])
	{
		out.num_segments = whitgl_imin(WORM_MAX_SEGMENTS, out.num_segments+3);
		out.has_ripple[in.num_segments-1] = false;
	}

	if(is_player)
	{
		out.vol_current.ground = (in.vol_current.ground*4 + out.vol_target.ground)/5;
		whitgl_loop_volume(SOUND_GROUND, out.vol_current.ground);
		if(out.vol_target.emerge > in.vol_current.emerge)
			out.vol_current.emerge = (in.vol_current.emerge + out.vol_target.emerge)/2;
		else
			out.vol_current.emerge = (in.vol_current.emerge*14 + out.vol_target.emerge)/15;
		whitgl_loop_volume(SOUND_EMERGE, out.vol_current.emerge);
		out.vol_current.shhh = (in.vol_current.shhh*9 + out.vol_target.shhh)/10;
		whitgl_loop_volume(SOUND_SHHH, out.vol_current.shhh);
		out.vol_current.shaaa = (in.vol_current.shaaa*9 + out.vol_target.shaaa)/10;
		whitgl_loop_volume(SOUND_SHAAA, out.vol_current.shaaa);
	}

	return out;
}
void worm_draw(ld29_worm worm, whitgl_ivec camera)
{
	if(!worm.alive)
		return;
	whitgl_sys_color normal_color = {0xf4, 0xc2, 0xde, 0xff};
	whitgl_sys_color hurt_color = {0xae, 0xb7, 0x46, 0xff};
	whitgl_sys_color pregnant_color = {0xf8, 0xaf, 0xb6, 0xff};
	int i;
	for(i=0; i<worm.num_segments; i++)
	{
		whitgl_fcircle c = whitgl_fcircle_zero;
		whitgl_sys_color color = normal_color;
		c.pos = whitgl_fvec_add(worm.segments[i], whitgl_ivec_to_fvec(camera));
		float ratio = ((float)worm.num_segments-i)/worm.num_segments;
		c.size = (whitgl_float)ratio*5+1;
		if(i%4==0) c.size *= 1.25;
		if(worm.has_ripple[i]) c.size *= 1.5;
		int pregnancy_pos = worm.num_segments*worm.pregnancy;
		if(worm.pregnancy > 0 && i > 0 && i == pregnancy_pos)
		{
			c.size += 10*worm.pregnancy;
			color = pregnant_color;
		}
		if(worm.hurt_segment != -1 && i >= worm.hurt_segment)
			color = hurt_color;

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
		whitgl_sys_draw_fcircle(mawc, normal_color, 16);

		maw_off = whitgl_angle_to_fvec(worm.dir-maw_ang_off*1.2);
		maw_off_scale.x = 6; maw_off_scale.y = 6;
		maw = whitgl_fvec_add(worm.segments[0], whitgl_fvec_scale(maw_off, maw_off_scale));
		mawc.pos = whitgl_fvec_add(maw, whitgl_ivec_to_fvec(camera));
		mawc.size = 2.5;
		whitgl_sys_draw_fcircle(mawc, normal_color, 16);

		maw_ang_off = -maw_ang_off;
	}
}

ld29_worm_ai worm_ai_update(ld29_worm_ai in, const ld29_worm* worm, const ld29_land* land)
{
	ld29_worm_ai out = in;
	out.repick = in.repick-1;
	if(out.repick <= 0)
	{
		out.repick = whitgl_randint(60*3);
		out.target.x = whitgl_randint(land->size.x);
		out.target.y = whitgl_randint(land->size.y-100);
		out.wiggle = 0;
		out.wiggle_speed = 0.1+whitgl_randfloat()/10;
	}
	out.wiggle += in.wiggle_speed;
	whitgl_fvec ideal_vector;
	whitgl_fvec ideal_vector_a = whitgl_fvec_sub(in.target, worm->segments[0]);
	ideal_vector = ideal_vector_a;
	whitgl_fvec ideal_vector_b = ideal_vector_a;
	ideal_vector_b.x -= land->size.x;
	if(whitgl_fvec_sqmagnitude(ideal_vector) > whitgl_fvec_sqmagnitude(ideal_vector_b))
		ideal_vector = ideal_vector_b;
	whitgl_fvec ideal_vector_c = ideal_vector_a;
	ideal_vector_c.x += land->size.x;
	if(whitgl_fvec_sqmagnitude(ideal_vector) > whitgl_fvec_sqmagnitude(ideal_vector_c))
		ideal_vector = ideal_vector_c;

	whitgl_float ideal_dir = whitgl_fvec_to_angle(ideal_vector) + whitgl_fsin(out.wiggle);
	whitgl_float actual_dir = worm->dir;
	float diff = ideal_dir - actual_dir;
	if(diff > whitgl_pi)
		diff -= whitgl_pi*2;
	if(diff < -whitgl_pi)
		diff += whitgl_pi*2;
	out.press_left = diff < 0;
	return out;
}
