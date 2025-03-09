#ifndef ORNAMENT_H
#define ORNAMENT_H

#include "parcer.h"

int ornament_rect_semicrcls(object_t*, int, char**);
int ornament_circle(object_t*, int, char**);
int parce_ornament(int argc, char** argv, object_t* figure);

#endif // ORNAMENT_H
