#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H

#include "../structures.h"

int get_padding(unsigned int);
bool write_bmp(const char*, const BitmapFileHeader*, const BitmapInfoHeader*, Rgb**);
bool read_bmp(const char*, BitmapFileHeader*, BitmapInfoHeader*, Rgb***);
void print_file_header(BitmapFileHeader);
void print_info_header(BitmapInfoHeader);



#endif //IMAGE_FUNCTIONS_H
