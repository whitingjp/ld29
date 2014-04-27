#ifndef WORM_H
#define WORM_H

#include <whitgl/math.h>

#include <entity/land.h>

#define WORM_MAX_SEGMENTS (128)
#define WORM_MIN_SEGMENTS (16)

typedef struct
{
	whitgl_fvec target;
	int repick;
	float wiggle;
	float wiggle_speed;
	bool press_left;
} ld29_worm_ai;
static const ld29_worm_ai ld29_worm_ai_zero = {{0,0}, 0, 0, 0.1, false};

typedef struct
{
	whitgl_float ground;
	whitgl_float emerge;
	whitgl_float shhh;
	whitgl_float shaaa;
} ld29_worm_volumes;
static const ld29_worm_volumes ld29_worm_volumes_zero = {0,0,0,0};

typedef struct
{
	whitgl_fvec segments[WORM_MAX_SEGMENTS];
	bool has_ripple[WORM_MAX_SEGMENTS];
	whitgl_fvec speed;
	whitgl_float dir;
	whitgl_float boost;
	whitgl_float boost_dir;
	whitgl_float maw_anim;
	ld29_worm_volumes vol_current;
	ld29_worm_volumes vol_target;
	whitgl_int air_time;
	whitgl_int num_segments;
	whitgl_int hurt_segment;
	whitgl_float pregnancy;
	bool alive;
	ld29_worm_ai ai;
	int ignore_hollows_counter;
} ld29_worm;
ld29_worm worm_zero(const ld29_land* land);
ld29_worm worm_update(ld29_worm in, const ld29_land* land, bool is_player, bool broody);
void worm_draw(ld29_worm w, bool is_player, whitgl_ivec camera);

ld29_worm_ai worm_ai_update(ld29_worm_ai in, const ld29_worm* worm, const ld29_land* land);

#endif // WORM_H