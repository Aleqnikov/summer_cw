#ifndef CANVAS_ONAMENT_H
#define CANVAS_ONAMENT_H

#include "image_functions.h"
#include "canvas_circle.h"
#include "canvas_rect.h"

void rectangle_ornament(Rgb***, BitmapInfoHeader, object_t, Rgb);
void circle_ornament(Rgb***, BitmapInfoHeader, Rgb);
void semi_circle_ornament(Rgb***, BitmapInfoHeader, int, int, Rgb);

#endif //CANVAS_ONAMENT_H