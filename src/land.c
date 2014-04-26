#include "land.h"

#include <whitgl/sys.h>

land land_zero()
{
	land out;
	int i;
	for(i=0; i<LAND_XRES*LAND_YRES; i++)
		out.data[i] = (i%2 ^ (i/5)%2)*255;
	return out;
}
void land_draw(const land* land)
{
	(void)land;
}
