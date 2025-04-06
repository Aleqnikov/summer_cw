#include "canvas_rotate.h"


/**
 * @brief Переворачивает часть изображения
 *
 * При помощи преобразований плоскости, мы меняем координаты точек, и затем ставим её на новое место.
 * Т.е x = xcosa + ysina, y = -xsina + ycosa
 */
Rgb** rotate_area(Rgb*** data, BitmapInfoHeader bmih, object_t figure){

    Rgb** new_data = copy_array(*data, bmih.height, bmih.width);
    if (new_data == NULL)
        return NULL;

    int x_center = (figure.x_left_up + figure.x_right_down)/2;
    int y_center = (figure.y_left_up + figure.y_right_down)/2;

    for (int y = figure.y_right_down; y <= figure.y_left_up; y++) {
        for (int x = figure.x_left_up; x <= figure.x_right_down; x++) {

            int x_shifted = x - x_center;
            int y_shifted = y - y_center;

            int x_new = (int)((x_shifted * custom_cos(figure.angle) + y_shifted * custom_sin(figure.angle)) + x_center);
            int y_new = (int)((-x_shifted *custom_sin(figure.angle)) + y_shifted * custom_cos(figure.angle) + y_center);

            if(check_coord(x_new, y_new, bmih.height, bmih.width))  new_data[y_new][x_new] = (*data)[y][x];

        }
    }

    for (int i = 0; i < (bmih.height); i++)
        free((*data)[i]);
    free(*data);

    return new_data;
}

