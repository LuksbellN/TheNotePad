#include "tnp_window_builder.h"

#include "tnp_font_size_handler.h"

/* DEFAULT VALUES */
enum {
        RECT_X = 200,
        RECT_Y = 10,
        RECT_WIDTH = 10,
        RECT_HEIGHT = 10,

        WIN_X = 10,
        WIN_Y = 10,
        WIN_WIDTH = 900,
        WIN_HEIGHT = 500,
        WIN_BORDER = 1,

        WIDTH_OFFSET = 8,
        HEIGHT_OFFSET = 12,

        WIDTH_DEFAULT = 10,
        HEIGHT_DEFAULT = 10
};

int main(){

        struct app_sizes sizes = (struct app_sizes){
                .window_x = WIN_X,
                .window_y = WIN_Y,
                .window_width = WIN_WIDTH,
                .window_height = WIN_HEIGHT,
                .window_border = WIN_BORDER,
                .width_offset = WIDTH_OFFSET,
                .height_offset = HEIGHT_OFFSET,
                .width_default = WIDTH_DEFAULT,
                .height_default = HEIGHT_DEFAULT
        };
        return tnp_window_builder_init(&sizes);

}
