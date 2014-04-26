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

void land_zero(land* land);
void land_splat(land* land, whitgl_fcircle c);
void land_draw(const land* land);

#endif // LAND_H