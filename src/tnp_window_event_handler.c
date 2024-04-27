#include <stdio.h>
#include <X11/X.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdbool.h>


#include "tnp_font_size_handler.h"
#include "tnp_window_builder.h"



void handle_key_pressed(Display *display, Window window, struct _XGC *gc, struct cursor_pos *cursor, char text[], int keycode, struct app_sizes *sizes);

void draw_key(Display *display, Window window, struct _XGC *gc, int width, int height, char text[]);

void get_character_typed(XIC xic, XKeyEvent *keycode, char text[]);

bool key_is_drawable_character(int keycode);







void handle_key_pressed(Display *display, Window window, struct _XGC *gc, struct cursor_pos *cursor, char text[], int keycode, struct app_sizes *sizes) {
        printf("%d", sizes->height_offset);
        if(key_is_drawable_character(keycode)){
                draw_key(display, window, gc, cursor->width_pos, cursor->height_pos, text);
                cursor->width_pos += sizes->width_offset;
        } else if(keycode == 36){
                cursor->height_pos += sizes->height_offset;
                cursor->width_pos = sizes->width_default;
        } else if(keycode == 22){
                cursor->width_pos -= sizes->width_offset;
                /* TODO: Clean character*/
                 XClearArea(display, window, cursor->width_pos, cursor->height_pos-10, sizes->width_offset, sizes->width_offset, 1);
        }                


}

void draw_key(Display *display, Window window, struct _XGC *gc, int width, int height, char text[]){

        XDrawString(display, window, gc, width, height, text, strlen(text));


}


void get_character_typed(XIC xic, XKeyEvent *keycode, char text[]){
        
        Status status;
        KeySym keysym = NoSymbol;
        
        Xutf8LookupString(xic, keycode, text, sizeof(&text) - 1, &keysym, &status);
        

}

bool key_is_drawable_character(int keycode){
        /* TODO: Implement the drawable charcater validator */

        return true;
}

