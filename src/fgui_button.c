/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

/**
 * @file
 *
 * fgui button module
 */

#include <stdint.h>
#include <string.h>

#include "fgui_event.h"
#include "fgui_widget.h"
#include "fgui_button.h"
#include "fgui_primitives.h"
#include "fgui_font.h"

#if 0
#define FGUI_BUTTON_BG_COLOR     FGUI_COLOR(0xcc, 0xcc, 0xcc)
#define FGUI_BUTTON_BORDER_COLOR FGUI_COLOR(0xaa, 0xaa, 0xaa)
#define FGUI_BUTTON_TEXT_COLOR   FGUI_COLOR(0, 0, 0)
#define FGUI_BUTTON_FOCUS_COLOR  FGUI_COLOR(255, 0, 0)
#else
#define FGUI_BUTTON_BG_COLOR     FGUI_COLOR(0xc0,0xc0,0xc0)
#define FGUI_BUTTON_BORDER_COLOR FGUI_COLOR(0x60, 0x60, 0x60)
#define FGUI_BUTTON_TEXT_COLOR   FGUI_COLOR(0, 0, 0)
#define FGUI_BUTTON_FOCUS_COLOR  FGUI_COLOR(255, 0, 0)

#define FGUI_BUTTON_TOP_COLOR    FGUI_COLOR(0xdc, 0xdc, 0xdc)
#define FGUI_BUTTON_TOQ_COLOR    FGUI_COLOR(0xd8, 0xd8, 0xd8)
#define FGUI_BUTTON_TOR_COLOR    FGUI_COLOR(0xc8, 0xc8, 0xc8)
#define FGUI_BUTTON_BOR_COLOR    FGUI_COLOR(0xb8, 0xb8, 0xb8)
#define FGUI_BUTTON_BOQ_COLOR    FGUI_COLOR(0xb0, 0xb0, 0xb0)
#define FGUI_BUTTON_BOT_COLOR    FGUI_COLOR(0xa8, 0xa8, 0xa8)
#endif

int fgui_button_init(struct fgui_button *button, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, const char *text, struct fgui_widget *parent)
{
	int ret;

	ret = fgui_widget_init(&button->base, x, y, w, h, parent);
	if (ret != 0) {
		return -1;
	}

	fgui_widget_set_draw(&button->base, fgui_button_draw);
	button->base.event_handler = fgui_button_event_handler;
	fgui_button_set_text(button, text);
	button->is_depressed = false;
	return 0;
}

void fgui_button_set_text(struct fgui_button *button, const char *text)
{
	strncpy(button->text, text, sizeof button->text);
	button->text[sizeof button->text - 1] = '\0';
}

void fgui_button_draw(struct fgui_widget *widget)
{
	struct fgui_button *button = (struct fgui_button *)widget;

	/* button background */
	fgui_fill_rectangle(button->base.area.x, button->base.area.y,
			button->base.area.w, button->base.area.h,
			FGUI_BUTTON_BG_COLOR);

	/* draw border, change color depending on focus */
	if (button->base.has_focus) {
		fgui_draw_rectangle(button->base.area.x, button->base.area.y,
				button->base.area.w, button->base.area.h,
				FGUI_BUTTON_FOCUS_COLOR);
	} else {
#if 0
		fgui_draw_rectangle(button->base.area.x, button->base.area.y,
				button->base.area.w, button->base.area.h,
				FGUI_BUTTON_BORDER_COLOR);
		fgui_draw_line(button->base.area.x, button->base.area.y,
				button->base.area.x+button->base.area.w, button->base.area.y,
				FGUI_BUTTON_TOP_COLOR);
		fgui_draw_line(button->base.area.x, button->base.area.y,
			       button->base.area.x, button->base.area.y+button->base.area.h,
			       FGUI_BUTTON_TOP_COLOR);
#else
		int x1 = button->base.area.x+2;
		int y1 = button->base.area.y;
		int x2 = x1+button->base.area.w-5;
		int y2 = y1+button->base.area.h-1;
		fgui_draw_line(x1, 1+y1, x2, 1+y1, FGUI_BUTTON_TOP_COLOR);
		fgui_draw_line(x1, 2+y1, x2, 2+y1, FGUI_BUTTON_TOQ_COLOR);
		fgui_draw_line(x1, 3+y1, x2, 3+y1, FGUI_BUTTON_TOR_COLOR);

		fgui_draw_line(x1, y2-3, x2, y2-3, FGUI_BUTTON_BOR_COLOR);
		fgui_draw_line(x1, y2-2, x2, y2-2, FGUI_BUTTON_BOQ_COLOR);
		fgui_draw_line(x1, y2-1, x2, y2-1, FGUI_BUTTON_BOT_COLOR);

		fgui_draw_line(x1-1, y1+2, x1-1, y2-2, FGUI_BUTTON_TOP_COLOR);
		fgui_draw_line(x2+1, y1+2, x2+1, y2-2, FGUI_BUTTON_BOT_COLOR);

		fgui_draw_line(x1, y1, x2, y1, FGUI_BUTTON_BORDER_COLOR);
		fgui_draw_line(x1, y2, x2, y2, FGUI_BUTTON_BORDER_COLOR);
		fgui_set_pixel(x1-1,y1+1, FGUI_BUTTON_BORDER_COLOR);
		fgui_set_pixel(x2+1,y1+1, FGUI_BUTTON_BORDER_COLOR);
		fgui_set_pixel(x1-1,y2-1, FGUI_BUTTON_BORDER_COLOR);
		fgui_set_pixel(x2+1,y2-1, FGUI_BUTTON_BORDER_COLOR);
		fgui_draw_line(x1-2, y1+2, x1-2, y2-2, FGUI_BUTTON_BORDER_COLOR);
		fgui_draw_line(x2+2, y1+2, x2+2, y2-2, FGUI_BUTTON_BORDER_COLOR);
#endif

	}

	/* button text */
	fgui_draw_string(button->text, button->base.area.x+2,
			button->base.area.y+2, FGUI_BUTTON_TEXT_COLOR, &button->base.area);

	/* draw extra thick border if button is depressed */
	if (button->is_depressed) {
		fgui_draw_rectangle(button->base.area.x-1,
				button->base.area.y-1, button->base.area.w+2,
				button->base.area.h+2,
				FGUI_BUTTON_FOCUS_COLOR);
	}

}

void fgui_button_set_on_click_handler(struct fgui_button *button,
		void (*callback)(void *userdata), void *userdata)
{
	button->on_click = callback;
	button->on_click_userdata = userdata;
}

int fgui_button_event_handler(struct fgui_widget *widget, struct fgui_event *event)
{
	struct fgui_button *button = (struct fgui_button *)widget;

	if (event->type == FGUI_EVENT_KEYUP && event->key.keycode == FGUI_KEY_SPACE) {
		if (button->on_click) {
			button->on_click(button->on_click_userdata);
		}
		button->is_depressed = false;
		return 0;
	}

	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == FGUI_KEY_SPACE) {
		button->is_depressed = true;
		return 0;
	}

	return 1;
}
