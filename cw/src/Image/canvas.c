/**
* @file canvas.c
* @brief Главная функция, которая рисует на изоборжаении.
* 
* Используется в главном файле, при рисовании на изображении.
*/

#include "canvas.h"

/**
 * @brief Главная функция, которая оперирует вспомогательными функциями, и рисует на изображении.
 * 
 * Используется в функции main.
 * @param figure фигура.
 */
int canvas_logic(object_t figure) {
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    Rgb **data = NULL;

    if(!read_bmp(figure.start_filename, &bmfh, &bmih, &data)){
        fprintf(stderr, "Error: Не удалось считать данные из файла.\n");
        return 1;
    }

    Rgb color_fill = (Rgb){figure.color_fill_b, figure.color_fill_g, figure.color_fill_r};
    Rgb color =  (Rgb){figure.color_b, figure.color_g, figure.color_r};

    figure.y_center = (int)bmih.height - figure.y_center;
    figure.y_left_up = (int)bmih.height - figure.y_left_up;
    figure.y_right_down = (int)bmih.height - figure.y_right_down;

    switch (figure.mod) {
        case rect:
            draw_rectangle(&data, bmih, figure, color_fill, color);
            break;
        case ornament:
            switch (figure.pattern) {
                case circle:
                    circle_ornament(&data, bmih, color);
                    break;
                case rectangle:
                    rectangle_ornament(&data, bmih, figure, color);
                    break;
                case semicircle:
                    semi_circle_ornament(&data, bmih, figure.count, figure.thinckness, color);
                    break;
            }
            break;
        case rotate:
            data = rotate_area(&data, bmih, figure);
            break;
        case circ:
            draw_circle(&data, bmih, color, color_fill, figure);
            break;
        case info:
            print_file_header(bmfh);
            print_info_header(bmih);
            break;
    }

    if (write_bmp(figure.finish_filename, &bmfh, &bmih, data)) 
        return 1;
    
    for (int i = 0; i < (bmih.height); i++) 
        free(data[i]);
    free(data);

    return 0;
}