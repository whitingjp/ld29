#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/math.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>

#include <land.h>
#include <worm.h>

land* l;

int main()
{
	WHITGL_LOG("Starting main.");

	whitgl_sys_setup setup = whitgl_default_setup;
	setup.name = "LD29";
	setup.size.x = 512;
	setup.size.y = 512;
	setup.pixel_size = 2;

	if(!whitgl_sys_init(setup))
		return 1;

	l = malloc(sizeof(land));
	land_zero(l);
	worm w = worm_zero(l);

	whitgl_sound_init();
	whitgl_input_init();

	double now = whitgl_sys_get_time();
	double then = now;
	double update_time = 0;

	bool running = true;
	while(running)
	{
		whitgl_sound_update();
		whitgl_input_update();

		then = now;
		now = whitgl_sys_get_time();
		update_time += now-then;
		//WHITGL_LOG("fps: %f", 1.0/(now-then));
		while(update_time > 0)
		{
			land_update(l);
			w = worm_update(w, l);
			whitgl_fcircle splat = whitgl_fcircle_zero;
			splat.pos = w.segments[5]; // can't do 0 because it mucks up land_filled
			splat.size = 40;
			land_splat(l, splat);			
			update_time -= 1.0/60.0;
		}

		whitgl_sys_draw_init();
		land_draw(l);
		worm_draw(w);
		whitgl_sys_draw_finish();

		if(whitgl_input_pressed(WHITGL_INPUT_ESC))
			running = false;
		if(whitgl_sys_should_close())
			running = false;
	}

	whitgl_input_shutdown();
	whitgl_sound_shutdown();

	free(l);

	whitgl_sys_close();

	return 0;
}
