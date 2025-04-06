#ifndef CANVAS_CIRCLE_H
#define CANVAS_CIRCLE_H

#include "image_functions.h"
#include "help_canvas_fns.h"

int circ_bre(int, point_t*);
int get_y_limit(circle_t, int, limit_circle_mod);
int draw_thicnless(Rgb***, circle_t, circle_t, BitmapInfoHeader, Rgb);
int draw_fill_circle(Rgb***, BitmapInfoHeader, circle_t , Rgb);
int draw_circle(Rgb***, BitmapInfoHeader, Rgb, Rgb, object_t);


#endif //CANVAS_CIRCLE_H
