#ifndef LAND_H
#define LAND_H

#include <whitgl/math.h>

#define LAND_XRES (1024)
#define LAND_YRES (512)
typedef struct
{
	whitgl_ivec size;
	unsigned char data[LAND_XRES*LAND_YRES*3];
} ld29_land;

void land_zero(ld29_land* land);
void land_update(ld29_land* land);
void land_splat(ld29_land* land, whitgl_fcircle c);
void land_draw(const ld29_land* land, whitgl_ivec camera);
bool land_filled(const ld29_land* land, whitgl_ivec p);

#endif // LAND_H