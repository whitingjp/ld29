#ifndef HUMAN_H
#define HUMAN_H

#include <whitgl/math.h>
#include <entity/egg.h>
#include <entity/land.h>

typedef struct
{
	bool alive;
	whitgl_fvec pos;
	bool going_left;
	whitgl_float anim;
} ld29_human;
ld29_human human_zero(whitgl_fvec pos);
ld29_human human_update(ld29_human in, const ld29_land* land);
void human_draw(ld29_human e, whitgl_ivec camera);

#endif // HUMAN_H