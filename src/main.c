#include <stdbool.h>
#include <stddef.h>

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/math.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>

#include <game.h>
#include <image.h>

ld29_game g;

int main()
{
	WHITGL_LOG("Starting main.");

	whitgl_sys_setup setup = whitgl_default_setup;
	setup.name = "LD29";
	setup.size.x = 512;
	setup.size.y = 256;
	setup.pixel_size = 2;
	// setup.fullscreen = true;

	if(!whitgl_sys_init(setup))
		return 1;

	whitgl_sys_add_image(IMAGE_SPRITES, "data/sprites.png");

	game_init(&g);

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
			game_update(&g);
			update_time -= 1.0/60.0;
		}

		whitgl_sys_draw_init();
		game_draw(&g, setup.size);
		whitgl_sys_draw_finish();

		if(whitgl_input_pressed(WHITGL_INPUT_ESC))
			running = false;
		if(whitgl_sys_should_close())
			running = false;
	}

	whitgl_input_shutdown();
	whitgl_sound_shutdown();

	whitgl_sys_close();

	return 0;
}
