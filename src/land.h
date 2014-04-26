#ifndef LAND_H
#define LAND_H

#include <whitgl/math.h>

#define LAND_XRES (512)
#define LAND_YRES (512)
typedef struct
{
	whitgl_ivec size;
	unsigned char data[LAND_XRES*LAND_YRES*3];
} land;

land land_zero();
void land_splat(land* land, whitgl_ivec pos);
void land_draw(const land* land);

#endif // LAND_H