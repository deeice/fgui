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
 * fgui checkbox module
 */

#include <stdint.h>
#include <string.h>

#include "fgui_event.h"
#include "fgui_widget.h"
#include "fgui_checkbox.h"
#include "fgui_primitives.h"
#include "fgui_font.h"

#define FGUI_CHECKBOX_BG_COLOR     FGUI_COLOR(255, 255, 255)
#define FGUI_CHECKBOX_BORDER_COLOR FGUI_COLOR(120,120,120)
#define FGUI_CHECKBOX_TEXT_COLOR   FGUI_COLOR(0, 0, 0)
#define FGUI_CHECKBOX_FOCUS_COLOR  FGUI_COLOR(255, 0, 0)
#define FGUI_CHECKBOX_CHECK_COLOR  FGUI_COLOR(48, 48, 152)
#define FGUI_CHECKBOX_CHEK_COLOR   FGUI_COLOR(48, 48, 192)

int fgui_checkbox_init(struct fgui_checkbox *checkbox, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, const char *text, struct fgui_widget *parent)
{
	int ret;

	ret = fgui_widget_init(&checkbox->base, x, y, w, h, parent);
	if (ret != 0) {
		return -1;
	}

	fgui_widget_set_draw(&checkbox->base, fgui_checkbox_draw);
	checkbox->base.event_handler = fgui_checkbox_event_handler;
	fgui_checkbox_set_text(checkbox, text);
	checkbox->is_checked = false;
	return 0;
}

void fgui_checkbox_set_text(struct fgui_checkbox *checkbox, const char *text)
{
	strncpy(checkbox->text, text, sizeof checkbox->text);
	checkbox->text[sizeof checkbox->text - 1] = '\0';
}

void fgui_checkbox_draw(struct fgui_widget *widget)
{
	struct fgui_checkbox *checkbox = (struct fgui_checkbox *)widget;
	int x1 = checkbox->base.area.x+5;
	int y1 = checkbox->base.area.y+5;
	int x2 = checkbox->base.area.x+15;
	int y2 = checkbox->base.area.y;

	fgui_fill_circle(x1, y1, 5, FGUI_CHECKBOX_BG_COLOR);

	if (checkbox->base.has_focus)
	  fgui_draw_circle(x1, y1, 5, FGUI_CHECKBOX_FOCUS_COLOR);
	else
	  fgui_draw_circle(x1, y1, 5, FGUI_CHECKBOX_BORDER_COLOR);
	if (checkbox->is_checked) {
	  fgui_fill_circle(x1, y1, 3, FGUI_CHECKBOX_CHECK_COLOR);
	  fgui_draw_circle(x1, y1, 3, FGUI_CHECKBOX_CHEK_COLOR);
	}
	fgui_draw_string(checkbox->text, x2, y2, FGUI_CHECKBOX_TEXT_COLOR, &checkbox->base.area);
}

void fgui_checkbox_set_on_click_handler(struct fgui_checkbox *checkbox,
		void (*callback)(void *userdata), void *userdata)
{
	checkbox->on_click = callback;
	if (userdata == NULL)
	  checkbox->on_click_userdata = (void *)&checkbox->is_checked;
	else
	  checkbox->on_click_userdata = userdata;
}

int fgui_checkbox_event_handler(struct fgui_widget *widget, struct fgui_event *event)
{
	struct fgui_checkbox *checkbox = (struct fgui_checkbox *)widget;

	if (event->type == FGUI_EVENT_KEYUP && event->key.keycode == FGUI_KEY_SPACE) {
		if (checkbox->on_click) {
			checkbox->on_click(checkbox->on_click_userdata);
		}
		return 0;
	}

	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == FGUI_KEY_SPACE) {
		checkbox->is_checked = !checkbox->is_checked;
		return 0;
	}

	return 1;
}
