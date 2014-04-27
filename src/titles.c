#include "titles.h"

#include <whitgl/sys.h>

#include <image.h>

void titles_draw(whitgl_float transition)
{
	whitgl_float trans_squared = transition*transition;
	whitgl_ivec frame = whitgl_ivec_zero;

	whitgl_sprite wyrmery_sprite = whitgl_sprite_zero;
	wyrmery_sprite.image = IMAGE_TITLES;
	wyrmery_sprite.size.x = 320; wyrmery_sprite.size.y = 124;
	whitgl_ivec wyrmery_draw_pos = whitgl_ivec_zero;
	wyrmery_draw_pos.y -= trans_squared*140;
	whitgl_sys_draw_sprite(wyrmery_sprite, frame, wyrmery_draw_pos);

	whitgl_sprite hints_sprite = whitgl_sprite_zero;
	hints_sprite.image = IMAGE_TITLES;
	hints_sprite.size.x = 140; hints_sprite.size.y = 110;
	hints_sprite.top_left.x = 0; hints_sprite.top_left.y = 126;
	whitgl_ivec hints_draw_pos = whitgl_ivec_zero;
	hints_draw_pos.x -= trans_squared*170; hints_draw_pos.y = 130;
	whitgl_sys_draw_sprite(hints_sprite, frame, hints_draw_pos);

	whitgl_sprite controls_sprite = whitgl_sprite_zero;
	controls_sprite.image = IMAGE_TITLES;
	controls_sprite.size.x = 160; controls_sprite.size.y = 110;
	controls_sprite.top_left.x = 142; controls_sprite.top_left.y = 126;
	whitgl_ivec controls_draw_pos = whitgl_ivec_zero;
	controls_draw_pos.x += 320-160+trans_squared*170; controls_draw_pos.y = 130;
	whitgl_sys_draw_sprite(controls_sprite, frame, controls_draw_pos);

	whitgl_sprite by_sprite = whitgl_sprite_zero;
	by_sprite.image = IMAGE_TITLES;
	by_sprite.size.x = 200; by_sprite.size.y = 40;
	by_sprite.top_left.x = 0; by_sprite.top_left.y = 237;
	whitgl_ivec by_draw_pos = whitgl_ivec_zero;
	by_draw_pos.x += 120; by_draw_pos.y = 200+70*trans_squared;
	whitgl_sys_draw_sprite(by_sprite, frame, by_draw_pos);
}