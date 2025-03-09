#ifndef ADDITIONAL_H
#define ADDITIONAL_H

#include "parcer.h"

bool isnumber(const char*);
bool is_correct_dots(const char*, int);
bool is_correct_count_args(int, char**, char*);
bool is_correct_coords(int, int, int);
int check_color(int, char**, char*);
int check_colors(int, int, int, int);
int parce_coords(int, char**, char*);
int left_up(object_t*, int, char**);
int right_down(object_t*, int, char**);
int thickness(object_t*, int, char**);
int color(object_t*, int, char**);
int fill_color(object_t*, int, char**);
int input_name(object_t*, int, char**);
int output_name(object_t*, int, char**);
void get_filename(object_t*, int, char**);
int angle(object_t*, int, char**);
int circle_get(object_t*, int, char**);
int radius(object_t*, int, char**);
int count(object_t*, int, char**);

#endif // ADDITIONAL_H
