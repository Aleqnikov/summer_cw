/**
* @file canvas_line.c
* @brief Файл, который содержит функию для рисовании линии на изображении.
*/

#include "canvas_line.h"

/**
 * @brief Данная функция является вспомогательной, рисует линию между двумя точками. Если линия имеет толщину,
 * то рисует её на t/2 вправо(вверх) влево(вниз) относительни линии. Используется алгоритм Брезенхема.
 * 
 * Используется при закрасики окружности, при рисовании прямоугольника. 
 * @param line Линия, хранит в себе информацию о двух точках, а также толщину линии.
 * @param data Массив пикселей.
 * @param bmih Информация о файле.
 * @param color Цвет линии.
 */
void draw_line(line_t line, Rgb*** data, BitmapInfoHeader bmih, Rgb color){
    
    int x_0 = get_value_range(min(line.point_1.x, line.point_2.x), 0, bmih.width, line.thickness / 2);
    int y_0 = get_value_range(min(line.point_1.y, line.point_2.y), 0, bmih.height, line.thickness / 2);

    int x_1 = get_value_range(max(line.point_1.x, line.point_2.x), 0, bmih.width, line.thickness / 2);
    int y_1 = get_value_range(max(line.point_1.y, line.point_2.y), 0, bmih.height, line.thickness / 2);

    int dx = x_1 - x_0;
    int dy = y_1 - y_0;
    int err = dx - dy;

    int sx = 1;
    int sy = 1;

    // Рискуем окружности на углах. Для 1 они просто и не нужны.
    if(line.thickness != 1){
        draw_circle(data, bmih, color, color, (object_t){.x_center = x_0, .y_center = y_0, .radius = line.thickness / 2, .thinckness = 1, .fill = true});
        draw_circle(data, bmih, color, color, (object_t){.x_center = x_1, .y_center = y_1, .radius = line.thickness / 2, .thinckness = 1, .fill = true});   
    }
    
    while (true) {
        if(line.thickness == 1){
            if(check_coord(y_0, x_0, bmih.height, bmih.width)) 
                (*data)[y_0][x_0] = color; 
        }else{
            for(int i = - line.thickness / 2; i <= line.thickness / 2; i++){
                if(x_0 == x_1){  // В зависимости от направления закрашиваем область. 
                    if(check_coord(y_0, x_0 + i, bmih.height, bmih.width)) (*data)[y_0][x_0 + i] = color;     
                } else
                    if(check_coord(y_0 + i, x_0, bmih.height, bmih.width)) (*data)[y_0 + i][x_0 ] = color;     
                
            }
        }
        
        if (x_0 == x_1 && y_0 == y_1)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x_0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y_0 += sy;
        }
    }
}
