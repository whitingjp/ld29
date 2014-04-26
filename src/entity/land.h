#ifndef LAND_H
#define LAND_H

#include <whitgl/math.h>

#define LAND_XRES (512)
#define LAND_YRES (512)
typedef struct land land;
struct land
{
	whitgl_ivec size;
	unsigned char data[LAND_XRES*LAND_YRES*3];
};

void land_zero(land* land);
void land_update(land* land);
void land_splat(land* land, whitgl_fcircle c);
void land_draw(const land* land);
bool land_filled(const land* land, whitgl_ivec p);

#endif // LAND_H