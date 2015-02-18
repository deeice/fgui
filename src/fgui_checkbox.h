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
 * fgui check box
 */

#ifndef FGUI_CHECKBOX_H
#define FGUI_CHECKBOX_H

#include <stdint.h>
#include <stdbool.h>

#include "fgui_event.h"
#include "fgui_widget.h"

#define MAX_TEXT_LEN 50

struct fgui_checkbox {
	struct fgui_widget base;
	char text[MAX_TEXT_LEN];
	void (*on_click)(void *userdata);
	void *on_click_userdata;
	bool is_checked;
};

int fgui_checkbox_init(struct fgui_checkbox *checkbox, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, const char *text, struct fgui_widget *parent);
void fgui_checkbox_set_text(struct fgui_checkbox *checkbox, const char *text);
void fgui_checkbox_draw(struct fgui_widget *widget);
void fgui_checkbox_set_on_click_handler(struct fgui_checkbox *checkbox,
		void (*callback)(void *userdata), void *userdata);
int fgui_checkbox_event_handler(struct fgui_widget *widget, struct fgui_event *event);

#endif /* FGUI_CHECKBOX_H */
