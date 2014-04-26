#ifndef LAND_H
#define LAND_H

#include <whitgl/math.h>

typedef enum
{
	LAND_SKY,
	LAND_GROUND,
	LAND_TUNNEL,
	LAND_MAX,
} ld29_land_type;

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

void land_set(ld29_land* land, ld29_land_type t, whitgl_ivec pos);
ld29_land_type land_get(const ld29_land* land, whitgl_ivec pos);

#endif // LAND_H