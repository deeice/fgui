/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

#include <stdint.h>
#include <stddef.h>    // size_t
#include <string.h>

#include "fgui_event.h"
#include "fgui_widget.h"
#include "fgui_combobox.h"
#include "fgui_primitives.h"
#include "fgui_font.h"

#if 0
#define FGUI_COMBOBOX_BG_COLOR        FGUI_COLOR(0xcc, 0xcc, 0xcc)
#define FGUI_COMBOBOX_BORDER_COLOR    FGUI_COLOR(0xaa, 0xaa, 0xaa)
#else
#define FGUI_COMBOBOX_BG_COLOR     FGUI_COLOR(0xc0,0xc0,0xc0)
#define FGUI_COMBOBOX_BORDER_COLOR FGUI_COLOR(0x60, 0x60, 0x60)
#define FGUI_COMBOBOX_TEXT_COLOR   FGUI_COLOR(0, 0, 0)
#define FGUI_COMBOBOX_FOCUS_COLOR  FGUI_COLOR(255, 0, 0)

#define FGUI_COMBOBOX_TOP_COLOR    FGUI_COLOR(0xdc, 0xdc, 0xdc)
#define FGUI_COMBOBOX_TOQ_COLOR    FGUI_COLOR(0xd8, 0xd8, 0xd8)
#define FGUI_COMBOBOX_TOR_COLOR    FGUI_COLOR(0xc8, 0xc8, 0xc8)
#define FGUI_COMBOBOX_BOR_COLOR    FGUI_COLOR(0xb8, 0xb8, 0xb8)
#define FGUI_COMBOBOX_BOQ_COLOR    FGUI_COLOR(0xb0, 0xb0, 0xb0)
#define FGUI_COMBOBOX_BOT_COLOR    FGUI_COLOR(0xa8, 0xa8, 0xa8)
#endif
#define FGUI_COMBOBOX_HIGHLIGHT_COLOR FGUI_COLOR(0x90, 0x90, 0x90)
#define FGUI_COMBOBOX_TEXT_COLOR      FGUI_COLOR(0, 0, 0)
#define FGUI_COMBOBOX_FOCUS_COLOR     FGUI_COLOR(255, 0, 0)



int fgui_combobox_init(struct fgui_combobox *combobox, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, struct fgui_widget *parent)
{
	int ret;

	ret = fgui_widget_init(&combobox->base, x, y, w, h, parent);
	if (ret != 0) {
		return -1;
	}

	fgui_widget_set_draw((struct fgui_widget *)combobox, fgui_combobox_draw);
	combobox->num_items = 0;
	combobox->current_item = -1;
	combobox->base.event_handler = fgui_combobox_event_handler;
	return 0;
}

int fgui_combobox_add_item(struct fgui_combobox *combobox, const char *text)
{
	size_t i;

	if (combobox->num_items >= MAX_COMBOBOX_ITEMS) {
		return -1;
	}

	i = combobox->num_items;
	strncpy(combobox->items[i].text, text, MAX_COMBOBOX_TEXT_LEN - 1);
	combobox->items[i].text[MAX_COMBOBOX_TEXT_LEN - 1] = '\0';
	combobox->num_items++;
	return 0;
}

int fgui_combobox_remove_item(struct fgui_combobox *combobox, size_t index)
{
	size_t i;

	if (index >= combobox->num_items) {
		return -1;
	}

	/* remove item by shifting the other items down one step */
	for (i = index; i < combobox->num_items - 1; i++) {
		combobox->items[i] = combobox->items[i + 1];
	}
	combobox->num_items--;
	return 0;
}

int fgui_combobox_current_index(struct fgui_combobox *combobox)
{
	return combobox->current_item;
}

void fgui_combobox_current_text(struct fgui_combobox *combobox, char *text)
{
	if (combobox->current_item < 0) {
		strcpy(text, "");
	} else {
		strncpy(text, combobox->items[combobox->current_item].text, MAX_COMBOBOX_TEXT_LEN - 1);
		text[MAX_COMBOBOX_TEXT_LEN - 1] = '\0';
	}
}

void fgui_combobox_set_index(struct fgui_combobox *combobox, size_t index)
{
	/* bound index to our available items */
	if (index < combobox->num_items) {
		combobox->current_item = index;
	} else {
		combobox->current_item = combobox->num_items - 1;
	}
}

void fgui_combobox_draw(struct fgui_widget *widget)
{
	size_t i;
	struct fgui_combobox *combobox = (struct fgui_combobox *)widget;
#if 0
	if (!combobox->base.has_focus) {
		combobox->is_expanded = false;
	}

	if (combobox->is_expanded) {
		/* combobox background */
		fgui_fill_rectangle(combobox->base.area.x, combobox->base.area.y,
				combobox->base.area.w, combobox->base.area.h * combobox->num_items,
				FGUI_COMBOBOX_BG_COLOR);

		/* highlight current item */
		fgui_fill_rectangle(combobox->base.area.x,
				combobox->base.area.y + combobox->base.area.h * combobox->current_item,
				combobox->base.area.w,
				combobox->base.area.h,
				FGUI_COMBOBOX_HIGHLIGHT_COLOR);

		/* border */
		fgui_draw_rectangle(combobox->base.area.x, combobox->base.area.y,
				combobox->base.area.w, combobox->base.area.h * combobox->num_items,
				FGUI_COMBOBOX_FOCUS_COLOR);

		for (i = 0; i < combobox->num_items; i++) {
			/* current item text */
			fgui_draw_string(combobox->items[i].text,
					combobox->base.area.x+2,
					combobox->base.area.y+2 + combobox->base.area.h * i,
					FGUI_COMBOBOX_TEXT_COLOR, NULL);
		}
	} else {
		/* combobox background */
		fgui_fill_rectangle(combobox->base.area.x, combobox->base.area.y,
				combobox->base.area.w, combobox->base.area.h,
				FGUI_COMBOBOX_BG_COLOR);

		/* border */
		fgui_draw_rectangle(combobox->base.area.x, combobox->base.area.y,
				combobox->base.area.w, combobox->base.area.h,
				FGUI_COMBOBOX_BORDER_COLOR);

		/* if focus, draw red border */
		if (combobox->base.has_focus) {
			fgui_draw_rectangle(combobox->base.area.x, combobox->base.area.y,
					combobox->base.area.w, combobox->base.area.h,
					FGUI_COMBOBOX_FOCUS_COLOR);
		}

		/* current item text */
		fgui_draw_string(combobox->items[combobox->current_item].text,
				combobox->base.area.x+2, combobox->base.area.y+2,
				FGUI_COMBOBOX_TEXT_COLOR, NULL);

		/* draw arrow to indicate that this is a combobox */
		fgui_draw_triangle(combobox->base.area.x + combobox->base.area.w - 15,
				combobox->base.area.y + combobox->base.area.h / 2 - 2,
				combobox->base.area.x + combobox->base.area.w - 5,
				combobox->base.area.y + combobox->base.area.h / 2 - 2,
				combobox->base.area.x + combobox->base.area.w - 10,
				combobox->base.area.y + combobox->base.area.h / 2 + 3,
				FGUI_COMBOBOX_BORDER_COLOR);
	}
#else
	int n = 1;
	int h = combobox->base.area.h*n;
	int x1 = combobox->base.area.x+2;
	int y1 = combobox->base.area.y;
	int x2 = x1+combobox->base.area.w-5;
	int y2 = y1+h-1;

	if (!combobox->base.has_focus) {
		combobox->is_expanded = false;
	} else if (combobox->is_expanded) {
		n = combobox->num_items;
		h = combobox->base.area.h*n;
		y2 = y1+h-1;
	}

	/* combobox background */
	fgui_fill_rectangle(combobox->base.area.x, combobox->base.area.y, combobox->base.area.w, h,
			FGUI_COMBOBOX_BG_COLOR);

	/* border */
	fgui_draw_line(x1, y1, x2, y1, FGUI_COMBOBOX_BORDER_COLOR);
	fgui_draw_line(x1, y2, x2, y2, FGUI_COMBOBOX_BORDER_COLOR);
	fgui_draw_line(x1-2, y1+2, x1-2, y2-2, FGUI_COMBOBOX_BORDER_COLOR);
	fgui_draw_line(x2+2, y1+2, x2+2, y2-2, FGUI_COMBOBOX_BORDER_COLOR);

	/* border shading */
	fgui_draw_line(x1, 1+y1, x2, 1+y1, FGUI_COMBOBOX_TOP_COLOR);
	fgui_draw_line(x1, 2+y1, x2, 2+y1, FGUI_COMBOBOX_TOQ_COLOR);
	fgui_draw_line(x1, 3+y1, x2, 3+y1, FGUI_COMBOBOX_TOR_COLOR);

	fgui_draw_line(x1, y2-1, x2, y2-1, FGUI_COMBOBOX_BOT_COLOR);
	fgui_draw_line(x1, y2-2, x2, y2-2, FGUI_COMBOBOX_BOQ_COLOR);
	fgui_draw_line(x1, y2-3, x2, y2-3, FGUI_COMBOBOX_BOR_COLOR);

	fgui_draw_line(x1-1, y1+2, x1-1, y2-2, FGUI_COMBOBOX_TOP_COLOR);
	fgui_draw_line(x2+1, y1+2, x2+1, y2-2, FGUI_COMBOBOX_BOT_COLOR);

	if (combobox->is_expanded) {
		/* highlight current item */
		fgui_fill_rectangle(x1-1, y1+1 + combobox->base.area.h * combobox->current_item,
				combobox->base.area.w-2,
				combobox->base.area.h-2,
				FGUI_COMBOBOX_HIGHLIGHT_COLOR);

		for (i = 0; i < combobox->num_items; i++) {
			/* current item text */
			fgui_draw_string(combobox->items[i].text,
				x1, combobox->base.area.y+2 + combobox->base.area.h * i,
				FGUI_COMBOBOX_TEXT_COLOR, NULL);
		}
	} else {
		/* if focus, draw red border */
		if (combobox->base.has_focus) {
			fgui_draw_rectangle(combobox->base.area.x, combobox->base.area.y,
					combobox->base.area.w, combobox->base.area.h,
					FGUI_COMBOBOX_FOCUS_COLOR);
		}

		/* current item text */
		fgui_draw_string(combobox->items[combobox->current_item].text,
				x1, combobox->base.area.y+2,
				FGUI_COMBOBOX_TEXT_COLOR, NULL);

		/* draw arrow to indicate that this is a combobox */
		fgui_draw_triangle(combobox->base.area.x + combobox->base.area.w - 15,
				combobox->base.area.y + combobox->base.area.h / 2 - 2,
				combobox->base.area.x + combobox->base.area.w - 5,
				combobox->base.area.y + combobox->base.area.h / 2 - 2,
				combobox->base.area.x + combobox->base.area.w - 10,
				combobox->base.area.y + combobox->base.area.h / 2 + 3,
				FGUI_COMBOBOX_BORDER_COLOR);
	}
	/* draw rounded border corners */
	fgui_set_pixel(x1-1,y1+1, FGUI_COMBOBOX_BORDER_COLOR);
	fgui_set_pixel(x2+1,y1+1, FGUI_COMBOBOX_BORDER_COLOR);
	fgui_set_pixel(x1-1,y2-1, FGUI_COMBOBOX_BORDER_COLOR);
	fgui_set_pixel(x2+1,y2-1, FGUI_COMBOBOX_BORDER_COLOR);
#endif
}

int fgui_combobox_event_handler(struct fgui_widget *widget, struct fgui_event *event)
{
	struct fgui_combobox *combobox = (struct fgui_combobox *)widget;

	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == FGUI_KEY_SPACE) {
		combobox->is_expanded = !combobox->is_expanded;
		return 0;
	}

	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == FGUI_KEY_ARROW_DOWN) {
		if (combobox->current_item < combobox->num_items - 1) {
			combobox->current_item++;
			if (combobox->on_change) {
				combobox->on_change(combobox->on_change_userdata);
			}
		}
		return 0;
	}

	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == FGUI_KEY_ARROW_UP) {
		if (combobox->current_item > 0) {
			combobox->current_item--;
			if (combobox->on_change) {
				combobox->on_change(combobox->on_change_userdata);
			}
		}
		return 0;
	}

	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == FGUI_KEY_RETURN) {
		combobox->is_expanded = false;
		return 0;
	}

	/* we didn't handle the event */
	return 1;
}

void fgui_combobox_set_on_change_handler(struct fgui_combobox *combobox,
		void (*callback)(void *userdata), void *userdata)
{
	combobox->on_change = callback;
	combobox->on_change_userdata = userdata;
}
