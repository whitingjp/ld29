#ifndef EGG_H
#define EGG_H

#include <whitgl/math.h>

#include <entity/land.h>

typedef struct
{
	whitgl_fvec pos;
	whitgl_fvec speed;
} egg;
egg egg_zero(const land* land);
egg egg_update(egg in, const land* land);
void egg_draw(egg e);

#endif // EGG_H