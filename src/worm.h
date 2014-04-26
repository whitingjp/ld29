#ifndef WORM_H
#define WORM_H

#include <whitgl/math.h>

#include <land.h>

#define WORM_NUM_SEGMENTS (48)
typedef struct
{
	whitgl_fvec segments[WORM_NUM_SEGMENTS];
	whitgl_fvec speed;
	whitgl_float dir;
} worm;
worm worm_zero(const land* land);
worm worm_update(worm in, const land* land);
void worm_draw(worm w);

#endif // WORM_H