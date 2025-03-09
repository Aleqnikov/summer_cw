#ifndef ADDITIONAL_H
#define ADDITIONAL_H

#include "parcer.h"

bool isnumber(const char*);
bool iscoords(const char*);

bool iscolor(const char *str);
int parce_coords(char* , int , int , char** , char* );
int left_up(object_t* , int , char** );
int right_down(object_t* , int , char** );
int thickness(object_t* , int , char** );
int color(object_t*, int, char**);
int fill_color(object_t*, int, char**);
int input_name(object_t*, int, char**);
int output_name(object_t*, int, char**);
void get_filename(object_t*, int, char**);
int angle(object_t*, int, char** );
int circle_get(object_t*, int, char** );
int radius(object_t*, int, char**);

#endif // ADDITIONAL_H
