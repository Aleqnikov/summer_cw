/**
* @file canvas_rotate.c
* @brief Файл, который содержит функия для поворота части изображения.
* 
* Используется при повороте изобрежения.
*/

#include "canvas_rotate.h"

/**
 * @brief Данная функция переворачивает часть изображения.
 *
 * При помощи преобразований плоскости, мы меняем координаты точек, и затем ставим её на новое место.
 * Т.е x = xcosa + ysina, y = -xsina + ycosa. Используется в файле canvas.ACCESSX_MAX_TABLESIZE
 * 
 * @param Получает на вход указатель на массив данных, фигуры, и InfoHeader.
 */
Rgb** rotate_area(Rgb*** data, BitmapInfoHeader bmih, object_t figure){

    Rgb** new_data = copy_array(*data, bmih.height, bmih.width);
    if (new_data == NULL)
        return NULL;

    
    int x_left = get_value_range(figure.x_left_up, 0, bmih.width);
    int x_right = get_value_range(figure.x_right_down, 0, bmih.width);
    
    int y_up = get_value_range(figure.y_left_up, 0, bmih.height);
    int y_down = get_value_range(figure.y_right_down, 0, bmih.height);

    int x_center = (x_left + x_right) / 2;
    int y_center = (y_up + y_down) / 2;


    for (int y = y_down; y <= y_up; y++) {
        for (int x = x_left; x <= x_right; x++) {
            
            int x_shifted = x - x_center;
            int y_shifted = y - y_center;

            int x_new = (int)((x_shifted * custom_cos(figure.angle) + y_shifted * custom_sin(figure.angle)) + x_center);
            int y_new = (int)((-x_shifted *custom_sin(figure.angle)) + y_shifted * custom_cos(figure.angle) + y_center);

            if(check_coord(y_new, x_new, bmih.height, bmih.width))  new_data[y_new][x_new] = (*data)[y][x];

        }
    }

    for (int i = 0; i < (bmih.height); i++)
        free((*data)[i]);
    free(*data);

    return new_data;
}