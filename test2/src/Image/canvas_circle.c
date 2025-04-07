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


/**
 * @brief Данная функия находит верхний или нижний предел координат при каком либо х в массиве.
 * 
 * Используется для получения границ закраски круга.
 * 
 * @param Получает на вход массив точек, текушее значение х и режим, может быть верхний или нижний.
 */
int get_y_limit(circle_t cirlce, int x_val, limit_circle_mod mode){
    int y = (mode == down? INT_MAX: INT_MIN);

    for (int i = 0; i < cirlce.len_array; i++) {

        if ((cirlce.points[i]).x + cirlce.x_center == x_val) {
            if (mode == down)
                y = y < (cirlce.points[i]  ).y ? y : (cirlce.points[i]).y;
            else
                y = y > (cirlce.points[i]  ).y ? y : (cirlce.points[i]).y;
        }
    }

    if (y == (mode == down ? INT_MAX: INT_MIN))
        return 0;
    return y;
}

/**
 * @brief Данная функия закрашивает область между двумя окружностями. Т.о является толщиной линии.
 * 
 * Используется при создании тощины линии.
 * 
 * @param Массив точек, массив малой и большой окружности,цвет и InfoHeader.
 */
int draw_thicnless(Rgb*** data, circle_t big_circle, circle_t min_circle, BitmapInfoHeader bmih, Rgb color) {
    int x_min = big_circle.x_center; int x_max = big_circle.radius + big_circle.x_center;
    int y_min = big_circle.y_center;

    for (int j = x_min; j <= x_max; j++) {
        int y_up_lim = get_y_limit(big_circle, j, up);
        int y_down_lim = get_y_limit(min_circle, j, down);

        for (int i = y_down_lim; i <= y_up_lim; i++) {
            if (check_coord(y_min + i, j, bmih.height, bmih.width))
                (*data)[y_min + i][j] = color;
            if (check_coord(y_min - i, j, bmih.height, bmih.width))
                (*data)[y_min - i][j] = color;
            if (check_coord(y_min + i, 2*x_min - j, bmih.height, bmih.width))
                (*data)[y_min + i][2*x_min - j] = color;
            if (check_coord(y_min - i, 2* x_min - j, bmih.height, bmih.width))
                (*data)[y_min - i][2* x_min - j] = color;
        }
    }
    return 1;
}

/**
 * @brief Данная функия закрашивает область внутри окружности.
 * 
 * Используется при закрашивания круга.
 * 
 * @param Массив точек, массив малой окружности,цвет и InfoHeader.
 */
int draw_fill_circle(Rgb*** data, BitmapInfoHeader bmih, circle_t circle , Rgb color) {
    int x_min = circle.x_center; int x_max = circle.radius + circle.x_center ;
    int y_min = circle.y_center;


    for (int j = x_min; j <= x_max; j++) {
        int y_up_lim = get_y_limit(circle, j, up);

        for (int i = 0; i <= y_up_lim; i++) {
            if (check_coord(y_min + i, j, bmih.height, bmih.width))
                (*data)[y_min + i][j] = color;
            if (check_coord(y_min - i, j, bmih.height, bmih.width))
                (*data)[y_min - i][j] = color;
            if (check_coord(y_min + i, 2*x_min - j, bmih.height, bmih.width))
                (*data)[y_min + i][2*x_min - j] = color;
            if (check_coord(y_min - i, 2* x_min - j, bmih.height, bmih.width))
                (*data)[y_min - i][2* x_min - j] = color;
        }
    }

    return 1;
}

/**
 * @brief Данная функия рисует окружность.
 * 
 * Используется при рисовании круга круга.
 * 
 * @param Массив точек,цвет, цвет заливки, InfoHeader, фигуру.
 */
int draw_circle(Rgb*** data, BitmapInfoHeader bmih, Rgb color, Rgb color_fill, object_t figure) {
    figure.thinckness = (figure.thinckness  % 2 == 0) ? figure.thinckness + 1 : figure.thinckness;

    int radius_min = figure.radius - figure.thinckness / 2;
    int radius_max = figure.radius + figure.thinckness / 2;

    circle_t min_circle = {
        .points = malloc(sizeof(point_t) * radius_min * 8),
        .radius = radius_min,
        .x_center = figure.x_center,
        .y_center = figure.y_center,
        .thickness = figure.thinckness,
    };

    circle_t big_circle = {
        .points = malloc(sizeof(point_t) * radius_max * 8),
        .radius = radius_max,
        .x_center = figure.x_center,
        .y_center = figure.y_center,
        .thickness = figure.thinckness,
    };

    min_circle.len_array = circ_bre(radius_min, min_circle.points);
    big_circle.len_array = circ_bre(radius_max, big_circle.points);

    if (figure.fill)
        draw_fill_circle(data, bmih, min_circle, color_fill);

    draw_thicnless(data, big_circle, min_circle, bmih, color);

    free(min_circle.points);
    free(big_circle.points);

    return 0;
}