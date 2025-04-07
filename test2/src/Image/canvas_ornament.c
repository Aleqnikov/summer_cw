/**
* @file canvas_ornament.c
* @brief Файл, который содержит функию для рисовании орнаментов на изображении.
* 
* Используется при рисовании орнаментов.
*/

#include "canvas_ornament.h"

/**
 * @brief Данная функция рисует орнамент прямоугольник на изображении.
 *
 * Используется как функция рисования орнамента.
 * 
 * @param Получает на вход указатель на массив данных, фигуры, и InfoHeader а также цвет.
 */
int rectangle_ornament(Rgb*** data, BitmapInfoHeader bmih, object_t figure, Rgb color) {
    int thickness = (figure.thinckness  % 2 == 0) ? figure.thinckness + 1 : figure.thinckness;
    int diff = thickness / 2;

    for (int i = 0; i < figure.count; i++) {

        const object_t tmp_figure = {
            .fill = figure.fill,
            .thinckness = figure.thinckness,
            .x_left_up = diff,
            .x_right_down = bmih.width - diff,
            .y_left_up = bmih.height - diff,
            .y_right_down = diff
        };

        draw_rectangle(data, bmih, tmp_figure, (Rgb){0,0,0}, color); // Второй цвет никогда не будет использован
        diff += 2*thickness;
    }
    return 1;
}

/**
 * @brief Данная функция рисует орнамент круг на изображении.
 *
 * Используется как функция рисования орнамента.
 * 
 * @param Получает на вход указатель на массив данных, фигуры, и InfoHeader а также цвет.
 */
int circle_ornament(Rgb*** data, BitmapInfoHeader bmih, Rgb color) {
    int x_c = (bmih.width + 1) / 2;
    int y_c = (bmih.height + 1) / 2;
    int min_radius = x_c < y_c ? x_c : y_c;
    int max_radius =  x_c + y_c;

    object_t tmp_figure = {
        .fill = false,
        .thinckness = 2*(max_radius - min_radius),
        .x_center = x_c,
        .y_center = y_c,
        .radius = max_radius,
    };

    draw_circle(data, bmih, color, (Rgb){0,0,0}, tmp_figure);

}

/**
 * @brief Данная функция рисует орнамент из нескольких кругов на изображении.
 *
 * Используется как функция рисования орнамента.
 * 
 * @param Получает на вход указатель на массив данных, фигуры, и InfoHeader а также цвет, количество и толщину линии.
 */
int semi_circle_ornament(Rgb*** data, BitmapInfoHeader bmih, int count, int thickness, Rgb color) {
    const int width_step = (bmih.width + count * 2 - 1) / (count * 2);
    const int height_step = (bmih.height + count * 2 - 1) / (count * 2);

    int current_x = width_step;
    int current_y = bmih.height - height_step - 1;

    const Rgb bg_color = {0, 0, 0};
    const bool no_fill = false;

    for (int i = 0; i < count; i++) {

        object_t horizontal_circle = {
            .thinckness = thickness,
            .fill = no_fill,
            .radius = width_step,
            .x_center = current_x,
            .y_center = 0  
        };
        draw_circle(data, bmih, color, bg_color, horizontal_circle);

        horizontal_circle.y_center = bmih.height;  
        draw_circle(data, bmih, color, bg_color, horizontal_circle);

        object_t vertical_circle = {
            .thinckness = thickness,
            .fill = no_fill,
            .radius = height_step,
            .x_center = 0,
            .y_center = current_y
        };
        draw_circle(data, bmih, color, bg_color, vertical_circle);

        vertical_circle.x_center = bmih.width;
        draw_circle(data, bmih, color, bg_color, vertical_circle);

        current_x += 2 * width_step;
        current_y -= 2 * height_step;
    }

    return 0;
}