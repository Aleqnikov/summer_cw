#ifndef PARCER_ASSETS_H
#define PARCER_ASSETS_H

#include "parcer.h"

bool parce_pattern(char*, object_t*);
bool parce_left_up(const char*, object_t*);
bool parce_right_down(const char*, object_t*);
bool parce_angle(const char*, object_t*);
bool parce_count(const char*, object_t*);
bool parce_thickness(const char*, object_t*);
bool parce_color(const char*, object_t*);
bool parce_fill_color(const char*, object_t*);
bool parce_center(const char*, object_t*);
bool parce_radius(const char*, object_t*);

#endif //PARCER_ASSETS_H
