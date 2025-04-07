/**
* @file canvas_rect.c
* @brief Файл, который содержит функию для рисовании прямоугольника на изображении.
* 
* Используется при рисовании прямоугольника.
*/

#include "canvas_rect.h"

/**
 * @brief Данная функция рисует прямоугольник на изображении.
 *
 * Используется как функция рисования.
 * 
 * @param Получает на вход указатель на массив данных, фигуры, и InfoHeader.
 */
void draw_rectangle(Rgb*** data, BitmapInfoHeader bmih, object_t figure, Rgb color_fill, Rgb color) {

    int thickness = figure.thinckness;
    int x_lu = figure.x_left_up; int x_rd = figure.x_right_down;
    int y_lu = figure.y_left_up; int y_rd = figure.y_right_down;

    if (figure.fill) {
        for(int y = y_rd; y <= y_lu ; y++) {
            for(int x = x_lu; x <= x_rd ; x++)
                if(check_coord(y, x, bmih.height, bmih.width)) (*data)[y][x] = color_fill;
        }
    }

    for (int upper = - (thickness) / 2; upper <= (thickness) / 2; upper++) {
        for (int side = - (thickness) / 2; side <= (thickness) / 2; side++) {
            for(int y = y_rd; y <= y_lu ; y++) {
                if(check_coord(y + upper, x_lu + side, bmih.height, bmih.width)) (*data)[y + upper][x_lu + side] = color;
                if(check_coord(y + upper, x_rd + side, bmih.height, bmih.width)) (*data)[y + upper][x_rd + side] = color;
            }

            for(int x = x_lu; x <= x_rd ; x++) {
                if(check_coord(y_lu + upper, x + side, bmih.height, bmih.width)) (*data)[y_lu + upper][x + side] = color;
                if(check_coord(y_rd + upper, x + side, bmih.height, bmih.width)) (*data)[y_rd + upper][x + side] = color;
            }
        }
    }
}
