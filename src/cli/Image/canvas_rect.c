#include "canvas_rect.h"

int draw_rectangle(Rgb*** data, BitmapInfoHeader bmih, bool fill, Rgb color_fill, Rgb color, int thickness, int x_lu, int x_rd, int y_lu, int y_rd) {
    if (fill) {
        for(int y = y_rd; y <= y_lu ; y++) {
            for(int x = x_lu; x <= x_rd ; x++) {
                if(check_coord(y, x, bmih.height, bmih.width)) (*data)[y][x] = color_fill;
            }
        }
    }

    // Рисуем обводку Сначала верхние полосы, затем боковые.
    for(int y = y_rd; y <= y_lu ; y++) {
        for (int upper = - (thickness) / 2; upper <= (thickness) / 2; upper++) {
            for (int side = - (thickness) / 2; side <= (thickness) / 2; side++) {
                if(check_coord(y + upper, x_lu + side, bmih.height, bmih.width)) (*data)[y + upper][x_lu + side] = color;
                if(check_coord(y + upper, x_rd + side, bmih.height, bmih.width)) (*data)[y + upper][x_rd + side] = color;
            }
        }
    }


    for(int x = x_lu; x <= x_rd ; x++) {
        for (int upper = - (thickness) / 2; upper <= (thickness) / 2; upper++) {
            for (int side = - (thickness) / 2; side <= (thickness) / 2; side++) {
                if(check_coord(y_lu + upper, x + side, bmih.height, bmih.width)) (*data)[y_lu + upper][x + side] = color;
                if(check_coord(y_rd + upper, x + side, bmih.height, bmih.width)) (*data)[y_rd + upper][x + side] = color;
            }
        }
    }


    return 0;

}
