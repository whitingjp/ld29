#ifndef WORM_H
#define WORM_H

#include <whitgl/math.h>

#define WORM_NUM_SEGMENTS (48)
typedef struct
{
	whitgl_fvec segments[WORM_NUM_SEGMENTS];
	whitgl_float dir;
} worm;

worm worm_zero();
worm worm_update(worm in);
void worm_draw(worm w);

#endif // WORM_H