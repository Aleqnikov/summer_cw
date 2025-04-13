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
 * @param rad Радиус окружности.
 * @param circle Массив для хранения точек окружности.
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
 * @brief Данная функция используется заполнения области внутри окружности цветом.
 * 
 * Для каждой из точек одного октанта, соединяет их линиями, перпендикулярными, паралельными.
 * Используется в функции рисования окружности.
 * 
 * @param data Массив пикселей.
 * @param circle Информация о круге, который заполняем.
 * @param bmih Информация о изображении.
 * @param color Цвет заливки.
 */
void draw_fill_circle(Rgb*** data, circle_t circle, BitmapInfoHeader bmih, Rgb color){
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

/**
 * @brief Данная функция является вспомогательной, она для всякого переданного x ищет самое большое значение 
 * из Y что есть на окружности. Есть два режима down, up первый используется для поиска границы маленькой окружности
 * а второй для большой.
 * 
 * Используется в функции рисования толщины линии.
 * 
 * @param circle Информация о круге, который заполняем.
 * @param x_val Значение x.
 * @param mode режим, есть верхний, или нижний. верхний используется для поиска в большой окружности, а 
 * нижний в маленькой.
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
 * @brief Данная функия рисует толщину линии для окружности.
 * Она просто закрашивает область между двумя переданными её окружнастями.
 * 
 * Используется как вспомогательная функция при рисовании окружности.
 * 
 * @param data Массив точек.
 * @param big_circle Информация о круге большом круге.
 * @param min_circle Информация о круге малом.
 * @param bmih Хэдер с информации о изображении.
 * @param color Цвет.
 */
void draw_thicnless(Rgb*** data, circle_t big_circle, circle_t min_circle, BitmapInfoHeader bmih, Rgb color) {
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
}

/**
 * @brief Данная функия рисует окружность.
 * 
 * Используется при рисовании круга круга.
 * 
 * @param data Массив точек.
 * @param bmih Хэдер с информации о изображении.
 * @param color Цвет.
 * @param color_fill цвет заливки.
 * @param figure Фигура с информацией об изображении.
 */
int draw_circle(Rgb*** data, BitmapInfoHeader bmih, Rgb color, Rgb color_fill, object_t figure) {

    circle_t circle = {
        .points = malloc(sizeof(point_t) * figure.radius * 8),
        .radius = figure.radius,
        .x_center = figure.x_center,
        .y_center = figure.y_center,
        .thickness = figure.thinckness,
    };

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

    circle.len_array = circ_bre(circle.radius, circle.points);
    

    if (figure.fill)
        draw_fill_circle(data, circle, bmih, color_fill);

    draw_thicnless(data, big_circle, min_circle, bmih, color);

        
    free(circle.points);
    free(min_circle.points);
    free(big_circle.points);
    return 0;
}