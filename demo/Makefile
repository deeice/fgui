# Makefile for fgui, a tiny gui for SDL
PREFIX := /usr/local
CC := gcc
CFLAGS := -g -Os -DUSE_SDL_TTF -fomit-frame-pointer -fno-strict-aliasing -ffunction-sections -fdata-sections 
SDL := -L/usr/lib -lSDL -lSDL_ttf
SDLFLAGS := `sdl-config --cflags`
SRC := ../src/fgui_application.c ../src/fgui_button.c ../src/fgui_combobox.c ../src/fgui_font.c ../src/fgui_label.c ../src/fgui_lineedit.c  ../src/fgui_checkbox.c ../src/fgui_primitives.c ../src/fgui_widget.c ../src/fonts/courier.c
OBJS := $(SRC:.c=.o)
INCLUDES := 
LVERSION := 2:1:1 # rules: http://www.gnu.org/software/libtool/manual/libtool.html#Updating-version-info

all: fguidemo

clean:
	rm -f $(LOBJS) fguidemo

fguidemo: fgui_demo.c $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SDLFLAGS) $(OBJS) fgui_demo.c $(LDFLAGS) -o fguidemo $(SDL)


