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
 * @param data Массив пикселей.
 * @param bmih Информация о файле.
 * @param color Цвет кругов.
 * @param figure Информация для рисования линий.
 */
void rectangle_ornament(Rgb*** data, BitmapInfoHeader bmih, object_t figure, Rgb color) {
    for (int i = 0; i < figure.count; i++) {

        int x_1 = 2 * i * figure.thinckness;
        int y_1 = 2 * i * figure.thinckness;
        int x_2 = bmih.width - 2 * i * figure.thinckness - 1;
        int y_2 = figure.thinckness + 2 * i * figure.thinckness - 1;
        int x_3 = bmih.width - figure.thinckness - 2 * i * figure.thinckness;
        int y_3 = bmih.height - 2 * i * figure.thinckness - 1;
        int x_4 = figure.thinckness + 2 * i * figure.thinckness - 1;
        int y_4 = bmih.height - 2 * i * figure.thinckness - 1;
        int x_5 = 2 * i * figure.thinckness;
        int y_5 = bmih.height - figure.thinckness - 2 * i * figure.thinckness;

        const object_t tmp_figure1 = { .fill = true, .thinckness = 0, .x_left_up = x_1, .x_right_down = x_2, .y_left_up = y_1, .y_right_down = y_2};
        const object_t tmp_figure2 = { .fill = true, .thinckness = 0, .x_left_up = x_3, .x_right_down = x_2, .y_left_up = y_3, .y_right_down = y_2};
        const object_t tmp_figure3 = { .fill = true, .thinckness = 0, .x_left_up = x_3, .x_right_down = x_5, .y_left_up = y_3, .y_right_down = y_5};
        const object_t tmp_figure4 = { .fill = true, .thinckness = 0, .x_left_up = x_1, .x_right_down = x_4, .y_left_up = y_1, .y_right_down = y_4};

        draw_rectangle(data, bmih, tmp_figure1, color, color);
        draw_rectangle(data, bmih, tmp_figure2, color, color);
        draw_rectangle(data, bmih, tmp_figure3, color, color);
        draw_rectangle(data, bmih, tmp_figure4, color, color);
    }
}

/**
 * @brief Данная функция рисует орнамент круг на изображении.
 *
 * Используется как функция рисования орнамента.
 * 
 * @param data Массив пикселей.
 * @param bmih Информация о файле.
 * @param color Цвет кругов.
 */
void circle_ornament(Rgb*** data, BitmapInfoHeader bmih, Rgb color) {
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
 * @param data Массив пикселей.
 * @param bmih Информация о файле.
 * @param color Цвет кругов.
 * @param thickness Толщина линии.
 * @param count Количество.
 */
void semi_circle_ornament(Rgb*** data, BitmapInfoHeader bmih, int count, int thickness, Rgb color) {
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
}