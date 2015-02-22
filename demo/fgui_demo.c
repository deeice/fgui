#include "SDL.h"
#include "../src/fgui.h"

#define WIDTH 320
#define HEIGHT 240

#define LINEHEIGHT 20 /* pixels */

// Screen surface
SDL_Surface *gScreen;
struct fgui_application app;

struct fgui_button button;
struct fgui_button button2;
struct fgui_label label;
struct fgui_combobox combobox;
struct fgui_lineedit lineedit;
struct fgui_checkbox checkbox;

struct fgui_listbox listbox;

// Simulate menubar with combobox to test how it looks.
#define fgui_menubar 		fgui_combobox
#define fgui_menubar_init	fgui_combobox_init
#define fgui_menubar_add_item	fgui_combobox_add_item
#define fgui_menubar_set_index	fgui_combobox_set_index
struct fgui_menubar menubar; 

// define the "callback" that fgui uses to set pixels
void fgui_set_pixel(uint16_t x, uint16_t y, uint32_t color)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = 1;
	r.h = 1;
	SDL_FillRect(gScreen, &r, color);
}

// test some fgui primitives
void render_stuff(void)
{
	/* draw background */
	fgui_fill_rectangle(0, 0, WIDTH, HEIGHT, FGUI_COLOR(221,221,221));

	fgui_draw_string("Use TAB to cycle focus", 100, 1*LINEHEIGHT, 0, NULL);

	fgui_draw_string("Line", 10, 2*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_line(160, 5+2*LINEHEIGHT, 220, 5+2*LINEHEIGHT, FGUI_COLOR(31,127,31));

	fgui_draw_string("Rectangles and Circles", 10, 3*LINEHEIGHT, 0, NULL);
	fgui_draw_rectangle(160, 3*LINEHEIGHT, 10, 10, FGUI_COLOR(0,0,255));
	fgui_fill_rectangle(160+20, 3*LINEHEIGHT, 10, 10, FGUI_COLOR(31, 191, 31));
	fgui_fill_rectangle(160+40, 3*LINEHEIGHT, 10, 10, FGUI_COLOR(31, 191, 31));
	fgui_draw_rectangle(160+40, 3*LINEHEIGHT, 10, 10, FGUI_COLOR(0,0,255));

	fgui_fill_circle(160+65, 5+3*LINEHEIGHT, 5, FGUI_COLOR(255, 255, 255));
	fgui_draw_circle(160+65, 5+3*LINEHEIGHT, 5, FGUI_COLOR(120,120,120));
	fgui_fill_circle(160+65, 5+3*LINEHEIGHT, 3, FGUI_COLOR(48, 48, 152));
	fgui_draw_circle(160+65, 5+3*LINEHEIGHT, 3, FGUI_COLOR(48, 48, 192));

	fgui_fill_circle(160+85, 5+3*LINEHEIGHT, 5, FGUI_COLOR(255, 255, 255));
	fgui_draw_circle(160+85, 5+3*LINEHEIGHT, 5, FGUI_COLOR(120,120,120));
	fgui_fill_circle(160+105, 5+3*LINEHEIGHT, 5, FGUI_COLOR(255, 255, 255));
	fgui_draw_circle(160+105, 5+3*LINEHEIGHT, 5, FGUI_COLOR(120,120,120));

	fgui_fill_rectangle(10, -6+4*LINEHEIGHT, 110, 2*LINEHEIGHT+2, FGUI_COLOR(255, 255,255));
	fgui_draw_string("this colored string\nspans multiple\nlines", 10, -4+4*LINEHEIGHT,
		FGUI_COLOR(255,0,0), NULL);

	fgui_draw_string("checkbox", 10, 6*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_string("push button", 10, 7*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_string("2nd push button", 10, 8*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_string("label widget", 10, 10*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_string("combobox", 10, 9*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_string("lineedit", 10, 11*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);

	fgui_application_draw(&app);
}

struct btn_cb_data {
	struct fgui_button *button;
	char *str;
	int *number;
};

void on_button_click(void *arg)
{
	char buf[30];
	struct btn_cb_data *data = arg;

	sprintf(buf, "clicks: %d", ++(*data->number));
	printf("button %p clicked\n", data->button);
	fgui_button_set_text(data->button, buf);
}

void on_combobox_change(void *userdata)
{
	printf("%s\n", __func__);
}

void on_checkbox_change(void *userdata)
{
  int *clicked = userdata;
  printf("%s = %s\n", __func__, *clicked ? "true" : "false");
}

void on_listbox_change(void *userdata)
{
	printf("%s\n", __func__);
}

enum fgui_key sdl_keysym_to_fgui_key(SDLKey sdl_keysym)
{
	enum fgui_key key;

	switch (sdl_keysym) {
		case SDLK_UP:    key = FGUI_KEY_ARROW_UP;    break;
		case SDLK_DOWN:  key = FGUI_KEY_ARROW_DOWN;  break;
		case SDLK_LEFT:  key = FGUI_KEY_ARROW_LEFT;  break;
		case SDLK_RIGHT: key = FGUI_KEY_ARROW_RIGHT; break;
		default:         key = sdl_keysym;           break;
	}

	return key;
}

int main(int argc, char *argv[])
{
	int ret;
	SDL_Event event;
	struct fgui_event fgui_event;
	int button_clicks = 0;
	int button2_clicks = 0;
	struct btn_cb_data btn_cb_data = {
		&button,
		"hello button userdata",
		&button_clicks,
	};

	struct btn_cb_data btn_cb_data2 = {
		&button2,
		"hello button2 userdata",
		&button2_clicks,
	};

	fgui_application_init(&app);
	fgui_listbox_init(&listbox, 160, 4*LINEHEIGHT-6, 60, 15, NULL);
	fgui_checkbox_init(&checkbox, 160, 6*LINEHEIGHT+3, 82, 15, "check it!", NULL);
	fgui_button_init(&button, 160, 7*LINEHEIGHT, 82, 15, "hello world", NULL);
	fgui_button_init(&button2, 160, 8*LINEHEIGHT, 82, 15, "hello world 2", NULL);
	fgui_combobox_init(&combobox, 160, 9*LINEHEIGHT, 60, 15, NULL);
	fgui_label_init(&label, 160, 10*LINEHEIGHT, 150, 20, "hello fgui label", NULL);
	fgui_lineedit_init(&lineedit, 160, 11*LINEHEIGHT, 100, 15, NULL);
	fgui_combobox_add_item(&combobox, "item1");
	fgui_combobox_add_item(&combobox, "item2");
	fgui_combobox_add_item(&combobox, "item3");
	fgui_combobox_set_index(&combobox, 0);
	fgui_listbox_add_item(&listbox, "item1");
	fgui_listbox_add_item(&listbox, "item2");
	fgui_listbox_add_item(&listbox, "item3");
	fgui_listbox_set_index(&listbox, 0);
	fgui_application_add_widget(&app, (struct fgui_widget *)&listbox);
	fgui_application_add_widget(&app, (struct fgui_widget *)&checkbox);
	fgui_application_add_widget(&app, &button.base);
	fgui_application_add_widget(&app, &button2.base);
	fgui_application_add_widget(&app, (struct fgui_widget *)&label);
	fgui_application_add_widget(&app, (struct fgui_widget *)&combobox);
	fgui_application_add_widget(&app, (struct fgui_widget *)&lineedit);
	fgui_button_set_on_click_handler(&button, on_button_click, &btn_cb_data);
	fgui_button_set_on_click_handler(&button2, on_button_click, &btn_cb_data2);
	fgui_combobox_set_on_change_handler(&combobox, on_combobox_change, NULL);
	fgui_checkbox_set_on_click_handler(&checkbox, on_checkbox_change, NULL);

	fgui_menubar_init(&menubar, 0, 0, 320, 15, NULL);
	fgui_menubar_add_item(&menubar, "Menubar");
	fgui_menubar_add_item(&menubar, "menuitem1");
	fgui_menubar_add_item(&menubar, "menuitem2");
	fgui_menubar_add_item(&menubar, "menuitem3");
	fgui_menubar_set_index(&menubar, 0);
	fgui_application_add_widget(&app, (struct fgui_widget *)&menubar);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// make sure things are cleaned up when we quit
	atexit(SDL_Quit);

	// attempt to create a 640x480 window with 32bit pixels.
	gScreen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE);
	if (gScreen == NULL) {
		fprintf(stderr, "Unable to set up video: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_EnableUNICODE(1);
	while (1) {
		render_stuff();

		// update the whole screen
		SDL_UpdateRect(gScreen, 0, 0, 0, 0);

		// blocking wait (tip: SDL_PollEvent is non-blocking)
		ret = SDL_WaitEvent(&event);
		if (ret == 0) {
			fprintf(stderr, "ERROR: SDL_WaitEvent: %s\n", SDL_GetError());
			exit(1);
		}

		switch (event.type) {
		case SDL_KEYDOWN:
			fgui_event.type = FGUI_EVENT_KEYDOWN;
			fgui_event.key.keycode = sdl_keysym_to_fgui_key(event.key.keysym.sym);
			if ((fgui_event.key.keycode >= 'a') && (fgui_event.key.keycode <= 'z'))
			{
			  if (event.key.keysym.mod & KMOD_SHIFT) 
			    fgui_event.key.keycode = toupper(fgui_event.key.keycode);
			}
			fgui_application_process_event(&app, &fgui_event);
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				// return (and thus, quit)
				return 0;
			default:
				fgui_event.type = FGUI_EVENT_KEYUP;
				fgui_event.key.keycode = sdl_keysym_to_fgui_key(event.key.keysym.sym);
				fgui_application_process_event(&app, &fgui_event);
				break;
			}
			break;

		case SDL_QUIT:
			return 0;

		default:
			break;
		}
	}
	return 0;
}
