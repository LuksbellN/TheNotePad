#include <X11/X.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <time.h>
#include <stdbool.h>
#include "tnp_window_builder.h"

#ifndef _TNP_WINDOW_EVENT_HANDLER_H
#define _TNP_WINDOW_EVENT_HANDLER_H


void handle_key_pressed(Display *display, Window window, struct _XGC *gc, struct cursor_pos *cursor, char text[], int keycode, struct app_sizes *sizes);

void draw_key(Display *display, Window window, struct _XGC *gc, int width, int height, char text[]);

void get_character_typed(XIC xic, XKeyEvent *keycode, char text[]);

bool key_is_drawable_character(int keycode);

#endif

