#include <X11/X.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <time.h>

enum {
        RECT_X = 200,
        RECT_Y = 10,
        RECT_WIDTH = 10,
        RECT_HEIGHT = 10,

        WIN_X = 10,
        WIN_Y = 10,
        WIN_WIDTH = 900,
        WIN_HEIGHT = 500,
        WIN_BORDER = 1
};

#define woffset 8
#define wdef 10
#define hoffset 10
#define hdef 10

typedef struct cursor_pos {
        int width_pos;
        int height_pos;
} cursor_pos;


void handle_key_pressed(Display *display, Window window, struct _XGC *gc, cursor_pos *cursor, char text[], int keycode);

 void draw_key(Display *display, Window window, struct _XGC *gc, int width, int height, char text[]);

int tnp_window_builder_init() {
        XEvent event;

        /* open connection with the server */
        Display *display = XOpenDisplay(NULL);
        if (display == NULL) {
                fprintf(stderr, "Cannot open display\n");
                exit(1);
        }

        int screen = DefaultScreen(display);

        /* create window */
        Window window = XCreateSimpleWindow(
                        display,
                        RootWindow(display, screen),
                        WIN_X, WIN_Y,
                        WIN_WIDTH, WIN_HEIGHT,
                        WIN_BORDER,
                        WhitePixel(display, screen),
                        WhitePixel(display, screen)
        );


        XSetLocaleModifiers("");

        XIM xim = XOpenIM(display, 0, 0, 0);
        if(!xim){
                // fallback to internal input method 
                XSetLocaleModifiers("@im=none");
                xim = XOpenIM(display, 0, 0, 0);

        }

        // X input context, you can have multiple for text boxes etc, but having a
        // single one is the easiest.

        XIC xic = XCreateIC(xim,
                        XNInputStyle,   XIMPreeditNothing | XIMStatusNothing,
                        XNClientWindow, window,
                        XNFocusWindow,  window,
                        NULL);

        XSetICFocus(xic);

        /* process window close event through event handler so XNextEvent does not fail */
        Atom del_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(display, window, &del_window, 1);

        /* select kind of events we are interested in */
        XSelectInput(display, window, ExposureMask | KeyPressMask);

        /* display the window */
        XMapWindow(display, window);
        
        struct _XGC *gc = XCreateGC(display, window, 0, 0);

        XSetBackground(display,gc,WhitePixel(display, screen));
        XSetForeground(display,gc,BlackPixel(display, screen));

        XClearWindow(display, window);
        
        cursor_pos cursor =  { .width_pos = wdef, .height_pos = hdef };
        /* event loop */
        while (1) {
                XNextEvent(display, &event);

                switch (event.type) {
                        case KeyPress:

                                Status status;
                                KeySym keysym = NoSymbol;
                                char text[32] = {};
                                Xutf8LookupString(xic, &event.xkey, text, sizeof(text) - 1, &keysym, &status);

                                handle_key_pressed(display, window, gc, &cursor, text, event.xkey.keycode);
   
                        case ClientMessage:
                                if(event.xkey.keycode == XKeysymToKeycode(display, XK_Escape)){
                                        goto breakout;
                                }
                        case Expose:
                                /* draw the window */
                                printf("\n");
                                /*
                                XFillRectangle(display, window, DefaultGC(display, screen), RECT_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT);*/

                        /* NO DEFAULT */
                }
        }
breakout:

        /* destroy window */
        XDestroyWindow(display, window);

        /* close connection to server */
        XCloseDisplay(display);
        XFreeGC(display, gc);
        return 0;

}


void handle_key_pressed(Display *display, Window window, struct _XGC *gc, cursor_pos *cursor, char text[], int keycode) {
        if(keycode == 36){
                cursor->height_pos += hoffset;
                cursor->width_pos = wdef;
        } else if(keycode == 22){
                cursor->width_pos -= woffset;
                /* TODO: Clean character*/
                 XClearArea(display, window, cursor->width_pos, cursor->height_pos-10, 12, 12, 1);
        } else {
                draw_key(display, window, gc, cursor->width_pos, cursor->height_pos, text);
                cursor->width_pos += woffset;
        }


}



 void draw_key(Display *display, Window window, struct _XGC *gc, int width, int height, char text[]){

        XDrawString(display, window, gc, width, height, text, strlen(text));


}



