#include "canvas.h"


int canvas_logic(object_t figure) {
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    Rgb **data = NULL;

    if(!read_bmp(figure.start_filename, &bmfh, &bmih, &data)){
        fprintf(stderr, "Error: Не удалось считать данные из файла.\n");
        return 0;
    }

    Rgb color_fill = (Rgb){figure.color_fill_b, figure.color_fill_g, figure.color_fill_g};
    Rgb color =  (Rgb){figure.color_b, figure.color_g, figure.color_r};

    switch (figure.mod) {
        case rect:
            draw_rectangle(&data, bmih, figure.fill, color_fill, color, figure.thinckness, figure.x_left_up, figure.x_right_down, figure.y_left_up, figure.y_right_down);
        break;
        case ornament:
            switch (figure.pattern) {
                case circle:
                    circle_ornament(&data, bmih, color);
                break;
                case rectangle:
                    rectangle_ornament(&data, bmih, figure.count, figure.thinckness, color);
                break;
                case semicircle:
                    semi_circle_ornament(&data, bmih, figure.count, figure.thinckness, color);
                break;
            }

        break;
        case rotate:
            data = rotate_area(&data, bmih, figure.angle, figure.x_left_up, figure.x_right_down, figure.y_left_up, figure.y_right_down);
        break;
        case circ:
            draw_circle(&data, bmih, color, color_fill, figure.thinckness, figure.radius, figure.fill, figure.x_center, figure.y_center);
        break;
        case info:
            print_file_header(bmfh);
        print_info_header(bmih);
        break;
    }


    // Записываем измененное изображение
    if (!write_bmp(figure.finish_filename, &bmfh, &bmih, data)) {
        return 1;
    }

    // Освобождаем память
    for (int i = 0; i < (bmih.height); i++) {
        free(data[i]);
    }
    free(data);

    return 0;
}
