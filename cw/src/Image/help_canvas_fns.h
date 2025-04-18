#ifndef HELP_CANVAS_FNS_H
#define HELP_CANVAS_FNS_H

#include "image_functions.h"

bool check_coord(int, int, int, int);
Rgb** copy_array(Rgb**, int, int);
int custom_cos(int);
int custom_sin(int angle);
int get_value_range(int, int, int, int);
int max(int , int);
int min (int, int);
int was_fill(int, int, int, int, BitmapInfoHeader);

#endif //HELP_CANVAS_FNS_H