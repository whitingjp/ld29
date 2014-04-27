#ifndef DAMAGE_H
#define DAMAGE_H

#include <whitgl/math.h>

typedef enum
{
	DAMAGE_NONE,
	DAMAGE_SIDE,
	DAMAGE_BLAST,
} ld29_damage_type;

typedef struct
{
	ld29_damage_type type;
	whitgl_fvec pos;
} ld29_damage;
static const ld29_damage damage_zero = {DAMAGE_NONE, {0,0}};

#endif // DAMAGE_H