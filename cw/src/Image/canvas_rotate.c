/**
* @file canvas_rotate.c
* @brief Файл, который содержит функия для поворота части изображения.
* 
* Используется при повороте изобрежения.
*/

#include "canvas_rotate.h"

/**
 * @brief Вспомогательная функция для расчёта сдвига части изображения.
 * 
 * Используется только при повороте части изображения.
 */
void get_diff(int* x, int* y, int angle, int weight_f, int heigt_f){
    switch (angle)
    {
    case 90:
        if(weight_f % 2 == 0 && heigt_f % 2 == 0){ *x = -1; *y = 0;}
        if(weight_f % 2 == 0 && heigt_f % 2 != 0){ *x = 0; *y = 1;}
        if(weight_f % 2 != 0 && heigt_f % 2 == 0){ *x = -1; *y = -1;}
        break;
    case 180:
        if(weight_f % 2 == 0 && heigt_f % 2 == 0){ *x = -1; *y = -1;}
        if(weight_f % 2 == 0 && heigt_f % 2 != 0){ *x = -1; *y = 0;}
        if(weight_f % 2 != 0 && heigt_f % 2 == 0){*x = 0; *y = -1;}
        break;

    case 270:
        if(weight_f % 2 == 0 && heigt_f % 2 == 0){ *x = 0; *y = -1;}
        if(weight_f % 2 == 0 && heigt_f % 2 != 0){ *x = 0; *y = 0;}
         
        if(weight_f % 2 != 0 && heigt_f % 2 == 0){ *x = 0; *y = -1;}
        break;
    
    default:
        break;
    }
}

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

    int x_left = min(figure.x_left_up, figure.x_right_down);
    int x_right = max(figure.x_left_up, figure.x_right_down);
    int y_up =  max(figure.y_left_up, figure.y_right_down);
    int y_down = min(figure.y_left_up, figure.y_right_down);

    x_left = get_value_range(x_left, 0, bmih.width, figure.thinckness / 2);
    x_right = get_value_range(x_right, 0, bmih.width, figure.thinckness / 2);
    
    y_up = get_value_range(y_up, 0, bmih.height, figure.thinckness / 2);
    y_down = get_value_range(y_down, 0, bmih.height, figure.thinckness / 2);

    int x_center = (x_left + x_right ) / 2;
    int y_center = (y_up + y_down) / 2;

    int diff_x = 0, diff_y = 0;

    get_diff(&diff_x, &diff_y, figure.angle, x_left + x_right, y_up + y_down);
    
    for (int y = y_down; y < y_up; y++) {
        for (int x = x_left; x < x_right; x++) {
            
            int x_shifted = x - x_center;
            int y_shifted = y - y_center;

            int x_new = (int)((x_shifted * custom_cos(figure.angle) + y_shifted * custom_sin(figure.angle)) + x_center + diff_x);
            int y_new = (int)((-x_shifted *custom_sin(figure.angle)) + y_shifted * custom_cos(figure.angle) + y_center + diff_y) ;

            if(check_coord(y_new, x_new, bmih.height, bmih.width))  new_data[y_new][x_new] = (*data)[y][x];

        }
    }

    for (int i = 0; i < (bmih.height); i++)
        free((*data)[i]);
    free(*data);

    return new_data;
}