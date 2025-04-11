/**
* @file canvas_circle.c
* @brief Файл, который содержит функию для рисовании круга на изображении, а также вспомогательные для этого функции.
* 
* Используется при рисовании круга.
*/

#include "canvas_circle.h"

/**
 * @brief Данная функция является реализацией алгоритма Брезенхема, добавляет точки в массив. Возвращает его длину.
 * 
 * Используется для получения координат окружности.
 * 
 * @param Получает на вход радиус и массив точек.
 */
int circ_bre(int rad, point_t* circle){
    int x = 0;
    int y = rad;
    int d = 3 - 2 * rad;

    int index = 0;

    while (x <= y){
        circle[index].x = x;
        circle[index].y = y;

        index++;

        if (x != y){
            circle[index].x = y;
            circle[index].y = x;
            index++;
        }

        if (d < 0) {
            d = d + 4 * x + 6;
        }
        else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    return index;
}

int draw_fill_circle(Rgb*** data, circle_t circle, BitmapInfoHeader bmih, Rgb color){


        for(int i = 0; i < circle.len_array; i++){
            int x_val = circle.points[i].x;
            int y_val = circle.points[i].y;

            point_t point1 = {circle.x_center + x_val, circle.y_center + y_val};
            point_t point2 = {circle.x_center + x_val, circle.y_center - y_val};
            point_t point3 = {circle.x_center - x_val, circle.y_center + y_val};
            point_t point4 = {circle.x_center - x_val, circle.y_center - y_val};

            point_t point5 = {circle.x_center + y_val, circle.y_center + x_val};
            point_t point6 = {circle.x_center + y_val, circle.y_center - x_val};
            point_t point7 = {circle.x_center - y_val, circle.y_center + x_val};
            point_t point8 = {circle.x_center - y_val, circle.y_center - x_val};


            
                draw_line((line_t){.thickness = 1, .point_1 = point1, .point_2 = point2}, data, bmih, color);
                draw_line((line_t){.thickness = 1, .point_1 = point2, .point_2 = point3}, data, bmih, color);
                draw_line((line_t){.thickness = 1, .point_1 = point3, .point_2 = point4}, data, bmih, color);       
                draw_line((line_t){.thickness = 1, .point_1 = point1, .point_2 = point4}, data, bmih, color);
            
    
            
                draw_line((line_t){.thickness = 1, .point_1 = point5, .point_2 = point6}, data, bmih, color);
                draw_line((line_t){.thickness = 1, .point_1 = point6, .point_2 = point7}, data, bmih, color);
                draw_line((line_t){.thickness = 1, .point_1 = point7, .point_2 = point8}, data, bmih, color);
                draw_line((line_t){.thickness = 1, .point_1 = point5, .point_2 = point8}, data, bmih, color);
            
            
        }
    

    
}

int draw_thincless_circle(Rgb*** data, circle_t* array, BitmapInfoHeader bmih, Rgb color){
    for(int i = 0; i < array->len_array; i++){
        int x_val = array->points[i].x;
        int y_val = array->points[i].y;

        int x_c = array->x_center;
        int y_c = array->y_center;

        object_t circle1 = {.radius = array->thickness / 2, .x_center = x_c + x_val, .y_center = y_c + y_val, .thinckness = 1, .fill = true, .thinckness = 0};
        object_t circle3 = {.radius = array->thickness / 2, .x_center = x_c - x_val, .y_center = y_c + y_val, .thinckness = 1, .fill = true, .thinckness = 0};
        object_t circle2 = {.radius = array->thickness / 2, .x_center = x_c + x_val, .y_center = y_c - y_val, .thinckness = 1, .fill = true, .thinckness = 0};
        object_t circle4 = {.radius = array->thickness / 2, .x_center = x_c - x_val, .y_center = y_c - y_val, .thinckness = 1, .fill = true, .thinckness = 0};

        object_t circle5 = {.radius = array->thickness / 2, .x_center = x_c + y_val, .y_center = y_c + x_val, .thinckness = 1, .fill = true, .thinckness = 0};
        object_t circle6 = {.radius = array->thickness / 2, .x_center = x_c + y_val, .y_center = y_c - x_val, .thinckness = 1, .fill = true, .thinckness = 0};
        object_t circle7 = {.radius = array->thickness / 2, .x_center = x_c - y_val, .y_center = y_c + x_val, .thinckness = 1, .fill = true, .thinckness = 0};
        object_t circle8 = {.radius = array->thickness / 2, .x_center = x_c - y_val, .y_center = y_c - x_val, .thinckness = 1, .fill = true, .thinckness = 0};

        draw_circle(data, bmih, (Rgb){0,0,0}, color, circle1);
        draw_circle(data, bmih,(Rgb){0,0,0}, color, circle2);
        draw_circle(data, bmih, (Rgb){0,0,0}, color, circle3);
        draw_circle(data, bmih, (Rgb){0,0,0}, color, circle4);
        draw_circle(data, bmih, (Rgb){0,0,0}, color, circle5);
        draw_circle(data, bmih, (Rgb){0,0,0}, color, circle6);
        draw_circle(data, bmih, (Rgb){0,0,0}, color, circle7);
        draw_circle(data, bmih, (Rgb){0,0,0}, color, circle8);
    }
}


/**
 * @brief Данная функия рисует окружность.
 * 
 * Используется при рисовании круга круга.
 * 
 * @param Массив точек,цвет, цвет заливки, InfoHeader, фигуру.
 */
int draw_circle(Rgb*** data, BitmapInfoHeader bmih, Rgb color, Rgb color_fill, object_t figure) {

    circle_t circle = {
        .points = malloc(sizeof(point_t) * figure.radius * 8),
        .radius = figure.radius,
        .x_center = figure.x_center,
        .y_center = figure.y_center,
        .thickness = figure.thinckness,
    };

    circle.len_array = circ_bre(circle.radius, circle.points);

    if (figure.fill)
        draw_fill_circle(data, circle, bmih, color_fill);

    if(figure.thinckness != 0)
        draw_thincless_circle(data, &circle, bmih, color);

    free(circle.points);
    return 0;
}