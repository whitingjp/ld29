#ifndef EGG_H
#define EGG_H

#include <whitgl/math.h>

#include <entity/land.h>

typedef struct
{
	whitgl_fvec pos;
	whitgl_fvec speed;
	bool dead;
} ld29_egg;
ld29_egg egg_zero(const ld29_land* land);
ld29_egg egg_update(ld29_egg in, const ld29_land* land);
void egg_draw(ld29_egg e, whitgl_ivec camera);

#endif // EGG_H