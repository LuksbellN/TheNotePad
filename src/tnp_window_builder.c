#include <X11/X.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <time.h>
#include <stdbool.h>


#include "tnp_font_size_handler.h"
#include "tnp_window_builder.h"
#include "tnp_window_event_handler.h"

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


struct cursor_pos create_cursor_pos(int width, int height) {
        return (struct cursor_pos){.width_pos = width, .height_pos = height};
};

int tnp_window_builder_init(struct app_sizes *sizes) {
        XEvent event;
        char text[32] = {};

        struct cursor_pos cursor =  create_cursor_pos(sizes->width_default, sizes->height_default);

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
        
        /* event loop */
        while (1) {
                XNextEvent(display, &event);

                switch (event.type) {
                        case KeyPress:

                                get_character_typed(xic, &event.xkey, text);
                                handle_key_pressed(display, window, gc, &cursor, text, event.xkey.keycode, sizes);
   
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


