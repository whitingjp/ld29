#ifndef DRILLER_H
#define DRILLER_H

#include <whitgl/math.h>
#include <entity/egg.h>
#include <entity/land.h>

#include <damage.h>

typedef struct
{
	whitgl_fvec pos;
	whitgl_fvec speed;
	float drill_timer;
	float beam_charge;
	ld29_damage attack;
} ld29_driller;
ld29_driller driller_zero(whitgl_fvec pos);
ld29_driller driller_update(ld29_driller in, const ld29_land* land, ld29_egg e);
void driller_draw(ld29_driller e);

#endif // DRILLER_H