#ifndef CANVAS_CIRCLE_H
#define CANVAS_CIRCLE_H

#include "image_functions.h"
#include "help_canvas_fns.h"

int circ_bre(int, point_t*);
int get_y_limit_up(point_t*, int, int, int);
int get_y_limit_down(point_t*, int, int, int);
int draw_thicnless(Rgb***, point_t*, point_t*, BitmapInfoHeader, int, int, Rgb, int, int, int, int);
int draw_fill_circle(Rgb***, point_t*, BitmapInfoHeader, int, int, Rgb, int, int);
int draw_circle(Rgb***, BitmapInfoHeader, Rgb, Rgb, int, int, int, int, int);


#endif //CANVAS_CIRCLE_H
