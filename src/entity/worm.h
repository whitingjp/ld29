#ifndef WORM_H
#define WORM_H

#include <whitgl/math.h>

#include <entity/land.h>

#define WORM_NUM_SEGMENTS (48)
typedef struct
{
	whitgl_fvec segments[WORM_NUM_SEGMENTS];
	whitgl_fvec speed;
	whitgl_float dir;
	whitgl_float boost;
	whitgl_float boost_dir;
	whitgl_float maw_anim;
} ld29_worm;
ld29_worm worm_zero(const ld29_land* land);
ld29_worm worm_update(ld29_worm in, const ld29_land* land);
void worm_draw(ld29_worm w, whitgl_ivec camera);

#endif // WORM_H