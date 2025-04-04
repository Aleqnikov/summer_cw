draw_circle(bmih, figure.radius, figure.thinckness, &data, figure.fill, color, color_fill, figure.x_center, figure.y_center);

// Function implementing Bresenham's circle algorithm for the entire first quarter of the circle
int circ_bre(int rad, point_t* circle){
    int x = 0;
    int y = rad;
    int d = 3 - 2 * rad;  // Initial decision parameter

    int index = 0;

    // Generate points for the first quarter (x >= 0, y >= 0, x² + y² ≈ r²)
    while (x <= y)
    {
        // Store the point in the first octant (x, y)
        circle[index].x = x;
        circle[index].y = y;
        index++;

        // Store the symmetric point in the second octant (y, x) — тоже в первой четверти!
        if (x != y)  // Чтобы не дублировать точку при x == y (на диагонали)
        {
            circle[index].x = y;
            circle[index].y = x;
            index++;
        }

        // Update decision parameter and coordinates
        if (d < 0)
        {
            d = d + 4 * x + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    return index;  // Return the number of points stored
}

int get_y_limit_up(point_t* circle, int len, int x_val, int x_c) {
    int y = INT_MIN;
    for (int i = 0; i <= len; i++) {
        if ((circle[i]).x + x_c == x_val)  y = y >(circle[i]).y ? y : (circle[i]).y;

    }

    if (y == INT_MIN)
        return 0;

    return y;
}


int get_y_limit_down(point_t* circle, int len, int x_val, int x_c) {
    int y = INT_MAX;
    for (int i = 0; i < len; i++) {

        if ((circle[i]).x + x_c == x_val) {
            y = y < (circle[i]).y ? y : (circle[i]).y;
        }
    }

    if (y == INT_MAX)
        return 0;
    return y;
}





int draw_thicnless(Rgb*** data, point_t* circle_m, point_t* circle_b, BitmapInfoHeader bmih, int rad,  int len_min, int len_big, Rgb color, int x_c, int y_c, int thickness) {
    int x_min = x_c; int x_max = rad + x_c + thickness / 2;
    int y_min = y_c; int y_max = rad + y_c + thickness / 2;


    for (int j = x_min; j <= x_max; j++) {

        int y_up_lim = get_y_limit_up(circle_b, len_big, j, x_c);
        int y_down_lim = get_y_limit_down(circle_m, len_min, j, x_c);

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

int draw_fill_circle(Rgb*** data, point_t* circle_m, BitmapInfoHeader bmih, int rad, int len_min, Rgb color, int x_c, int y_c) {
    int x_min = x_c; int x_max = rad + x_c ;
    int y_min = y_c; int y_max = rad + y_c;


    for (int j = x_min; j <= x_max; j++) {

        int y_up_lim = get_y_limit_up(circle_m, len_min, j, x_c);

        for (int i = 0; i <= y_up_lim; i++) {
            if (check_coord(y_min + i, j, bmih.height, bmih.width))
                (*data)[y_min + i][j] = color;
            if (check_coord(y_min - i, j,  bmih.height, bmih.width))
                (*data)[y_min - i][j] = color;
            if (check_coord(y_min + i, 2*x_min - j, bmih.height, bmih.width))
                (*data)[y_min + i][2*x_min - j] = color;
            if (check_coord(y_min - i, 2* x_min - j,  bmih.height, bmih.width))
                (*data)[y_min - i][2* x_min - j] = color;
        }
    }

    return 1;
}

int draw_circle(BitmapInfoHeader bmih, int radius, int thinckness, Rgb*** data, bool fill, Rgb color, Rgb color_fill, int x_c, int y_c) {
    thinckness = (thinckness  % 2 == 0) ? thinckness + 1 : thinckness;

    point_t min_circle[(radius - thinckness / 2 ) * 8]; // Оценка сверху количества точек
    point_t big_circle[(radius - thinckness/ 2 )*8];

    int len_min = circ_bre(radius - thinckness / 2, min_circle);
    int len_big = circ_bre(radius + thinckness / 2, big_circle);

    if (fill)
        draw_fill_circle(data, min_circle, bmih, radius - thinckness / 2, len_min, color_fill, x_c, y_c);

    draw_thicnless(data, min_circle, big_circle, bmih, radius, len_min, len_big, color, x_c, y_c, thinckness);

    return 0;

}





bool check_draw_thincless(int y, int x, int thincless, int x_min, int x_max, int y_min, int y_max){
    if(y >= y_min - thincless && y <= y_max + thincless && x >= x_min - thincless && x < x_min)
        return 1;

    if(y >= y_min - thincless && y <= y_max + thincless && x > x_max  && x <= x_max + thincless)
        return 1;

    if(x >= x_min - thincless && x <= x_max + thincless && y >= y_min - thincless && y < y_min)
        return 1;

    if(x >= x_min - thincless && x <= x_max + thincless && y > y_max  && y <= y_max + thincless)
        return 1;

    return 0;
}

int draw_rectangle(object_t info_m){
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    Rgb **data = NULL;

    if(!read_bmp(info_m.start_filename, &bmfh, &bmih, &data)){
        fprintf(stderr, "Error: Не удалось считать данные из файла.\n");
        return 0;
    }

    Rgb color = {info_m.color_b, info_m.color_g, info_m.color_r};
    Rgb color_thinckless = {info_m.color_fill_b, info_m.color_fill_g, info_m.color_fill_r};

    for(int y = info_m.y_right_down - info_m.thinckness; y <= info_m.y_left_up + info_m.thinckness; y++){
        for(int x = info_m.x_left_up - info_m.thinckness; x <= info_m.x_right_down + info_m.thinckness; x++){

            if(check_draw_thincless(y, x, info_m.thinckness, info_m.x_left_up, info_m.x_right_down, info_m.y_right_down, info_m.y_left_up) && check_coord(y, x, bmih.height, bmih.width))
                    data[y][x] = color_thinckless;

            if(info_m.fill){
                if(x <= info_m.x_right_down && x >= info_m.x_left_up && y >= info_m.y_right_down && y <= info_m.y_left_up && check_coord(y, x, bmih.height, bmih.width))
                    data[y][x] = color;
            }

        }
    }

    // Записываем измененное изображение
    if (!write_bmp(info_m.finish_filename, &bmfh, &bmih, data)) {
        return 1;
    }

    // Освобождаем память
    for (int i = 0; i < (bmih.height); i++) {
        free(data[i]);
    }
    free(data);

    return 0;
}


// Draw circle brezenchem method
int get_circle_coords(int r, point_t* circle) {

    int x = 0;
    int y = r;
    int delta = 1 - 2 * r;
    int error = 0;

    int index = 0;
    while(y >= 0) {
        circle[index].x = x; circle[index].y = y;
        index++;
        error = 2 * (delta + y) - 1;
        if(delta < 0 && error <= 0) {
            ++x;
            delta += 2 * x + 1;
            continue;
        }
        if(delta >= 0 && error > 0) {
            --y;
            delta += 1 - 2 * y;
            continue;
        }
        ++x;
        delta += 2 * (x - y);
        --y;
    }

    return index;
}


int bresenxemm(object_t figure, Rgb*** data, BitmapInfoHeader* bmih) {

    int x = 0;
    int y = figure.radius;
    int d = 3 - 2 * figure.radius;  // Initial decision parameter

    int index = 0;

    // Generate points for the first quarter (x >= 0, y >= 0, x² + y² ≈ r²)
    while (x <= y)
    {


        for (int j = - (figure.thinckness + 1) / 2; j < (figure.thinckness + 1) / 2; j++) {
            for (int i = - (figure.thinckness + 1) / 2; i < (figure.thinckness + 1) / 2; i++) {
                int x_c  = figure.x_center ;
                int y_c  = figure.y_center ;

                if (check_coord( y_c + y + j, x_c + x + i, bmih->height, bmih->width))  (*data)[y_c + y + j][x_c + x + i] =  (Rgb){figure.color_b, figure.color_g, figure.color_r};
                if (check_coord( y_c + x + j, x_c + y + i, bmih->height, bmih->width))  (*data)[y_c + x + j][x_c + y + i] =  (Rgb){figure.color_b, figure.color_g, figure.color_r};
                if (check_coord( y_c - x + j, x_c + y + i, bmih->height, bmih->width))  (*data)[y_c - x + j][x_c + y + i] =  (Rgb){figure.color_b, figure.color_g, figure.color_r};
                if (check_coord( y_c - y + j, x_c + x + i, bmih->height, bmih->width))  (*data)[y_c - y + j][x_c + x + i] =  (Rgb){figure.color_b, figure.color_g, figure.color_r};

                if (check_coord( y_c - y + j, x_c - x + i, bmih->height, bmih->width))  (*data)[y_c - y + j][x_c - x + i] =  (Rgb){figure.color_b, figure.color_g, figure.color_r};
                if (check_coord( y_c + x + j, x_c - y + i, bmih->height, bmih->width))  (*data)[y_c + x + j][x_c - y + i] =  (Rgb){figure.color_b, figure.color_g, figure.color_r};
                if (check_coord( y_c - x + j, x_c - y + i, bmih->height, bmih->width))  (*data)[y_c - x + j][x_c - y + i] =  (Rgb){figure.color_b, figure.color_g, figure.color_r};
                if (check_coord( y_c + y + j, x_c - x + i, bmih->height, bmih->width))  (*data)[y_c + y + j][x_c - x + i] =  (Rgb){figure.color_b, figure.color_g, figure.color_r};


            }
        }

        // Update decision parameter and coordinates
        if (d < 0)
        {
            d = d + 4 * x + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
    return index;
}

int bresenxemm2(object_t figure, Rgb*** data, BitmapInfoHeader* bmih) {

    int x = 0;
    int y = figure.radius;
    int d = 3 - 2 * figure.radius;  // Initial decision parameter

    int index = 0;

    // Generate points for the first quarter (x >= 0, y >= 0, x² + y² ≈ r²)
    while (x <= y)
    {

        if (figure.fill) {


            // Первый прямоугольник
            for (int j = figure.x_center - y; j <= figure.x_center + y; j++) {
                for (int i = figure.y_center - x; i <= figure.y_center + x; i++) {
                    if (check_coord(i, j, bmih->height, bmih->width)) {
                        (*data)[i][j] =  (Rgb){figure.color_fill_b, figure.color_fill_g, figure.color_fill_r};
                    }
                }
            }

            for (int j = figure.x_center - x; j <= figure.x_center + x; j++) {
                for (int i = figure.y_center - y; i <= figure.y_center + y; i++) {
                    if (check_coord(i, j, bmih->height, bmih->width)) {
                        (*data)[i][j] =  (Rgb){figure.color_fill_b, figure.color_fill_g, figure.color_fill_r};
                    }
                }
            }
        }


        // Update decision parameter and coordinates
        if (d < 0)
        {
            d = d + 4 * x + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    return index;

}



int draw_circle_4(object_t figure) {
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    Rgb **data = NULL;

    if(!read_bmp(figure.start_filename, &bmfh, &bmih, &data)){
        fprintf(stderr, "Error: Не удалось считать данные из файла.\n");
        return 0;
    }

    Rgb color = {figure.color_b, figure.color_g, figure.color_r};
    Rgb color_thinckless = {figure.color_fill_b, figure.color_fill_g, figure.color_fill_r};

    bresenxemm2(figure, &data, &bmih);
    bresenxemm(figure, &data, &bmih);


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


int draw_cicle_2(object_t figure) {
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    Rgb **data = NULL;

    if(!read_bmp(figure.start_filename, &bmfh, &bmih, &data)){
        fprintf(stderr, "Error: Не удалось считать данные из файла.\n");
        return 0;
    }

    Rgb color = {figure.color_b, figure.color_g, figure.color_r};
    Rgb color_thinckless = {figure.color_fill_b, figure.color_fill_g, figure.color_fill_r};

    // Применяем алгоритм Брезенхэма


        int x = 0;
        int y = figure.radius;
        int delta = 1 - 2 * figure.radius;
        int error = 0;

        int y_c = figure.y_center;
        int x_c = figure.x_center;

        while(y >= 0) {
            data[y_c + y][x_c + x] = color;
            data[y_c - y][x_c + x] = color;
            data[y_c + y][x_c - x] = color;
            data[y_c - y][x_c - x] = color;
            error = 2 * (delta + y) - 1;
            if(delta < 0 && error <= 0) {
                ++x;
                delta += 2 * x + 1;
                continue;
            }
            if(delta >= 0 && error > 0) {
                --y;
                delta += 1 - 2 * y;
                continue;
            }
            ++x;
            delta += 2 * (x - y);
            --y;
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

int draw_circle(object_t info_m){


    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    Rgb **data = NULL;

    if(!read_bmp(info_m.start_filename, &bmfh, &bmih, &data)){
        fprintf(stderr, "Error: Не удалось считать данные из файла.\n");
        return 0;
    }

    Rgb color = {info_m.color_b, info_m.color_g, info_m.color_r};
    Rgb color_thinckless = {info_m.color_fill_b, info_m.color_fill_g, info_m.color_fill_r};

    for(int y = info_m.y_center - info_m.radius - info_m.thinckness + 1; y < info_m.y_center + info_m.radius + info_m.thinckness; y++){
        for(int x = info_m.x_center - info_m.radius - info_m.thinckness + 1; x < info_m.x_center + info_m.radius + info_m.thinckness; x++){

            int radius_pre =(x - info_m.x_center)*(x - info_m.x_center) + (y - info_m.y_center)*(y - info_m.y_center);
            int min_rad =  info_m.radius*info_m.radius;
            int max_ras = (info_m.radius + info_m.thinckness)*(info_m.radius + info_m.thinckness);


            if(radius_pre <= max_ras && radius_pre >= min_rad && check_coord(y, x, bmih.height, bmih.width))
                data[y][x] = color_thinckless;


            if(info_m.fill){
                if(radius_pre < min_rad && check_coord(y, x, bmih.height, bmih.width))
                    data[y][x] = color;
            }

        }
    }

    // Записываем измененное изображение
    if (!write_bmp(info_m.finish_filename, &bmfh, &bmih, data)) {
        return 1;
    }

    // Освобождаем память
    for (int i = 0; i < (bmih.height); i++) {
        free(data[i]);
    }
    free(data);

    return 0;
}
