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
    int x_0 = min(figure.x_left_up, figure.x_right_down); 
    int x_1 = max(figure.x_left_up, figure.x_right_down);
    int y_0 = min(figure.y_left_up, figure.y_right_down);
    int y_1 = max(figure.y_left_up, figure.y_right_down);

    // Остявляем такой какая есть
    if (figure.fill) {
        for(int y = y_0; y <= y_1 ; y++) {
            for(int x = x_0; x <= x_1 ; x++)
                if(check_coord(y, x, bmih.height, bmih.width)) (*data)[y][x] = color_fill;
        }
    }

    point_t point1 = {x_0, y_0};
    point_t point2 = {x_0, y_1};
    point_t point3 = {x_1, y_1};
    point_t point4 = {x_1, y_0};


    draw_line((line_t){.thickness = figure.thinckness, .point_1 = point1, .point_2 = point2}, data, bmih, color);
    draw_line((line_t){.thickness = figure.thinckness, .point_1 = point2, .point_2 = point3}, data, bmih, color);
    draw_line((line_t){.thickness = figure.thinckness, .point_1 = point3, .point_2 = point4}, data, bmih, color);
    draw_line((line_t){.thickness = figure.thinckness, .point_1 = point1, .point_2 = point4}, data, bmih, color);
}
