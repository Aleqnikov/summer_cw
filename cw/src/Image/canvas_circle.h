#ifndef CANVAS_CIRCLE_H
#define CANVAS_CIRCLE_H

#include "image_functions.h"
#include "help_canvas_fns.h"
#include "canvas_line.h"

int circ_bre(int, point_t*);
int draw_fill_circle(Rgb***, circle_t, BitmapInfoHeader, Rgb);
int draw_circle(Rgb***, BitmapInfoHeader, Rgb, Rgb, object_t);
int draw_thincless_circle(Rgb***, circle_t*, BitmapInfoHeader, Rgb);

#endif //CANVAS_CIRCLE_H
