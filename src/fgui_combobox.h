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
 * fgui combo box
 *
 * Currently it behaves like a drop-down list. It may grow editing capabilities
 * later.
 */

#ifndef FGUI_COMBOBOX_H
#define FGUI_COMBOBOX_H

#include <stdint.h>
#include <stddef.h>    // size_t

#include "fgui_event.h"

#define MAX_COMBOBOX_TEXT_LEN 20
#define MAX_COMBOBOX_ITEMS 10

struct fgui_combobox_item {
	char text[MAX_COMBOBOX_TEXT_LEN];
};

struct fgui_combobox {
	struct fgui_widget base;
	struct fgui_combobox_item items[MAX_COMBOBOX_ITEMS];
	size_t num_items;
	int current_item;
	bool is_expanded;
	void (*on_change)(void *userdata);
	void *on_change_userdata;
};

int fgui_combobox_init(struct fgui_combobox *combobox, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, struct fgui_widget *parent);
int fgui_combobox_add_item(struct fgui_combobox *combobox, const char *text);
int fgui_combobox_remove_item(struct fgui_combobox *combobox, size_t index);
int fgui_combobox_current_index(struct fgui_combobox *combobox);
void fgui_combobox_current_text(struct fgui_combobox *combobox, char *text);
void fgui_combobox_set_index(struct fgui_combobox *combobox, size_t index);
void fgui_combobox_draw(struct fgui_widget *widget);
int fgui_combobox_event_handler(struct fgui_widget *widget, struct fgui_event *event);
void fgui_combobox_set_on_change_handler(struct fgui_combobox *combobox,
		void (*callback)(void *userdata), void *userdata);

#define fgui_listbox fgui_combobox
#define fgui_listbox_add_item fgui_combobox_add_item
#define fgui_listbox_set_index fgui_combobox_set_index
int fgui_listbox_event_handler(struct fgui_widget *widget, struct fgui_event *event);
int fgui_listbox_init(struct fgui_listbox *listbox, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, struct fgui_widget *parent);
void fgui_listbox_draw(struct fgui_widget *widget);

#endif /* FGUI_COMBOBOX_H */
