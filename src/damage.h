#ifndef DAMAGE_H
#define DAMAGE_H

#include <whitgl/math.h>

typedef enum
{
	DAMAGE_NONE,
	DAMAGE_SIDE,
} damage_type;

typedef struct
{
	damage_type type;
	whitgl_fvec pos;
} damage;
static const damage damage_zero = {DAMAGE_NONE, {0,0}};

#endif // DAMAGE_H