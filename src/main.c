#include <stdbool.h>
#include <stddef.h>

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/math.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>

#include <game.h>
#include <image.h>
#include <sounds.h>

ld29_game g;

const char* fragment_src = "\
#version 150\
\n\
in vec2 Texturepos;\
out vec4 outColor;\
uniform sampler2D tex;\
void main()\
{\
	float off = 0.01;\
	vec2 north = Texturepos; north.y -= off;\
	vec4 northt = texture( tex, north ); northt = floor(northt*1.1)/2.0;\
	vec2 east = Texturepos; east.x += off;\
	vec4 eastt = texture( tex, east ); eastt = floor(eastt*1.1)/2.0;\
	vec2 south = Texturepos; south.y += off;\
	vec4 southt = texture( tex, south ); southt = floor(southt*1.1)/2.0;\
	vec2 west = Texturepos; west.x -= off;\
	vec4 westt = texture( tex, west ); westt = floor(westt*1.1)/2.0;\
	outColor = texture( tex, Texturepos )+northt/8.0+eastt/8.0+southt/8.0+westt/8.0;\
	float dim = (1-Texturepos.y)/4;\
	outColor -= vec4(dim, dim*0.5, dim*0.8, 0);\
}\
";

int main()
{
	WHITGL_LOG("Starting main.");

	whitgl_sys_setup setup = whitgl_default_setup;
	setup.name = "Wyrmery";
	setup.size.x = 320;
	setup.size.y = 240;
	setup.pixel_size = 2;
	setup.fullscreen = false;

	if(!whitgl_sys_init(&setup))
		return 1;

	whitgl_sys_add_image(IMAGE_SPRITES, "data/sprites.png");

	WHITGL_LOG("Sound init");
	whitgl_sound_init();
	whitgl_sound_add(SOUND_OM0, "data/om0.ogg");
	whitgl_sound_add(SOUND_OM1, "data/om1.ogg");
	whitgl_sound_add(SOUND_OM2, "data/om2.ogg");
	whitgl_sound_add(SOUND_OM3, "data/om3.ogg");
	whitgl_loop_add(SOUND_GROUND, "data/ground.ogg");
	whitgl_loop_add(SOUND_EMERGE, "data/emerge.ogg");
	whitgl_loop_add(SOUND_SHHH, "data/shhh.ogg");
	whitgl_loop_add(SOUND_SHAAA, "data/shaaa.ogg");
	whitgl_sound_add(SOUND_POP, "data/pop.ogg");
	whitgl_sound_add(SOUND_TRIGGER, "data/trigger.ogg");
	whitgl_loop_add(SOUND_DRILL, "data/drill.ogg");
	whitgl_sound_add(SOUND_ITS_AN_EGG, "data/its_an_egg.ogg");
	whitgl_sound_add(SOUND_CRACK, "data/crack.ogg");

	whitgl_shader shader = whitgl_shader_zero;
	shader.fragment_src = fragment_src;
	whitgl_change_shader(WHITGL_SHADER_POST, shader);

	WHITGL_LOG("Game init");
	game_init(&g);

	WHITGL_LOG("Input init");
	whitgl_input_init();

	double now = whitgl_sys_get_time();
	double then = now;
	double update_time = 0;

	WHITGL_LOG("RNG init");
	whitgl_randseed(now);

	WHITGL_LOG("Running game.");
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
			game_update(&g, setup.size);
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
