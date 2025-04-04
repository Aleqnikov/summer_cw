#ifndef ADDITIONAL_H
#define ADDITIONAL_H

#include "parcer.h"

bool is_number(const char*);
bool is_correct_dots(const char*, int);
bool no_arguments(int, char**, char*);
bool is_correct_count_args(int, char**, char*, char*);
bool comp_cnt_args(int, int, char*);
bool check_color(int, char**, char*, char*);
bool check_colors(int, int, int, int);
bool parce_coords(int, char**, char*, char*);
bool left_up(object_t*, int, char**);
bool right_down(object_t*, int, char**);
bool thickness(object_t*, int, char**);
bool color(object_t*, int, char**);
bool fill_color(object_t*, int, char**);
bool input_name(object_t*, int, char**);
bool output_name(object_t*, int, char**);
void get_filename(object_t*, int, char**);
bool angle(object_t*, int, char**);
bool circle_get(object_t*, int, char**);
bool radius(object_t*, int, char**);
bool count(object_t*, int, char**);

#endif // ADDITIONAL_H
