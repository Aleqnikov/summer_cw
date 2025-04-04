#include "canvas_rotate.h"


/**
 * @brief Переворачивает часть изображения
 *
 * При помощи преобразований плоскости, мы меняем координаты точек, и затем ставим её на новое место.
 * Т.е x = xcosa + ysina, y = -xsina + ycosa
 */
Rgb** rotate_area(Rgb*** data, BitmapInfoHeader bmih, int angle, int x_lu, int x_rd, int y_lu, int y_rd){

    Rgb** new_data = copy_array(*data, bmih.height, bmih.width);
    int x_center = (x_lu + x_rd)/2;
    int y_center = (y_lu + y_rd)/2;


    for (int y = y_rd; y <= y_lu; y++) {
        for (int x = x_lu; x <= x_rd; x++) {
            // Смещаем пиксель относительно центра
            int x_shifted = x - x_center;
            int y_shifted = y - y_center;

            // Поворот
            int x_new = (int)((x_shifted * custom_cos(angle) + y_shifted * custom_sin(angle)) + x_center);
            int y_new = (int)((-x_shifted *custom_sin(angle)) + y_shifted * custom_cos(angle) + y_center);

            if(check_coord(x_new, y_new, bmih.height, bmih.width))  new_data[y_new][x_new] = (*data)[y][x];

        }
    }

    // Освобождаем память
    for (int i = 0; i < (bmih.height); i++)
        free((*data)[i]);
    free(*data);

    return new_data;
}