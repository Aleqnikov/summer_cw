#include "canvas_line.h"


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

    while (1) {
        if(line.thickness == 1){
            if(check_coord(y_0, x_0, bmih.height, bmih.width)) 
            (*data)[y_0][x_0] = color;
        } else{
            draw_circle(data, bmih, color, color, (object_t){.x_center = x_0, .y_center = y_0, .radius = line.thickness / 2, .thinckness = 0, .fill = true});
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
