#ifndef WHITGL_INPUT_H_
#define WHITGL_INPUT_H_

#include <stdbool.h>

#include <whitgl/math.h>

typedef enum
{
	// game
	WHITGL_INPUT_UP = 0,
	WHITGL_INPUT_RIGHT,
	WHITGL_INPUT_DOWN,
	WHITGL_INPUT_LEFT,
	WHITGL_INPUT_ESC,
	WHITGL_INPUT_A,
	WHITGL_INPUT_B,
	WHITGL_INPUT_X,
	WHITGL_INPUT_Y,
	WHITGL_INPUT_START,
	WHITGL_INPUT_SELECT,
	WHITGL_INPUT_ANY,
	WHITGL_INPUT_MAX,
} whitgl_input;

void whitgl_input_init();
void whitgl_input_shutdown();
void whitgl_input_update();
bool whitgl_input_down(whitgl_input input);
bool whitgl_input_pressed(whitgl_input input);
whitgl_ivec whitgl_input_mouse_pos(int pixel_size);

#endif // WHITGL_INPUT_H_