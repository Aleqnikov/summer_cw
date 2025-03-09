#ifndef PARCER_H
#define PARCER_H

#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum Pattern {rectangle, circle, semicircle} pattern_t;

typedef enum Mode {rect, ornament, rotate, circ} mode;


typedef struct Object {
    mode mod;

    char* start_filename;
    char* finish_filename;

    int x_left_up, y_left_up;
    int x_right_down, y_right_down;

    int thinckness;

    int color_r, color_g, color_b;

    bool fill;

    int color_fill_r, color_fill_g, color_fill_b;

    pattern_t pattern;

    int count;

    int angle;

    int x_center, y_center;
    int radius;
} object_t ;



#include "rectangle.h"
#include "circle.h"
#include "ornament.h"
#include "rotate.h"
#include "additional.h"


int base_parser(int, char** );

#endif // PARCER_H
