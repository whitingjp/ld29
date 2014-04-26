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
	damage attack;
} driller;
driller driller_zero(whitgl_fvec pos);
driller driller_update(driller in, const land* land, egg e);
void driller_draw(driller e);

#endif // DRILLER_H