/**
* @file image_functionss.c
 * @brief  Содержит  функции считывания и записи изображения, а также функции для
 * вывода информации о изображении.
 * 
 * Используется в файле canvas.c.
 */

#include "image_functions.h"

/**
 * @breif Данная функция получает на вход длину изображения и возвращает значение паддинга.
 * 
 * Используется при записи и считывания изображения.
 */
int get_padding(unsigned int width){
    return ((width*3 + 3) & (~3));
}

/**
* @brief Данная функция считывает изображение.
* 
* Используется для считывания изображения.
* 
* @param Получает название файла,указатель на Header файла, и Header информации файла, указатель на массив.
*/
bool read_bmp(const char* filename, BitmapFileHeader* file_header, BitmapInfoHeader* info_header, Rgb*** data){
    FILE* file = fopen(filename, "rb");

    if(!file){
        fprintf(stderr, "Error: Не удалось открыть файл!\n");
        return 0;
    }

    fread(file_header, sizeof(BitmapFileHeader), 1, file);
    fread(info_header, sizeof(BitmapInfoHeader), 1, file);

    if(file_header->signature != 0x4D42){
        fprintf(stderr, "Error: Файл не является bmp.\n");
        return 0;
    }

    if(info_header->bitsPerPixel != 24){
        fprintf(stderr, "Error: Программа поддерживает только 24 бита на пиксель!\n");
        return 0;
    }


    if(info_header->compression != 0){
        fprintf(stderr, "Error: Программа не обрабатывает изображения со сжатием!\n");
        return 0;
    }

    fseek(file, file_header->pixelArrOffset, SEEK_SET);
    int row_padded = get_padding(info_header->width);

    *data = (Rgb** )(malloc(sizeof(Rgb* )* info_header->height));

    if(!*data){
        fprintf(stderr, "Error: Не удалось выделить память под массив пикселей!\n");
        return 0;
    }

    for (int i = 0; i < info_header->height; i++){
        (*data)[i] = (Rgb *)malloc(info_header->width * sizeof(Rgb));

        if(!(*data)[i]){
            fprintf(stderr, "Error: Не удалось выделить память под строку пикселей!.\n");
            return 0;
        }
    }

    for(int i = 0; i < info_header->height; i++){
        for(int j = 0; j < info_header->width; j++)
            fread(&(*data)[i][j], sizeof(Rgb), 1, file);
        
        fseek(file, row_padded - info_header->width*3, SEEK_CUR);       // Пропускаем лишние данные выравнивания.
    }

    fclose(file);
    return 1;
}

/**
 * @brief Данная функция записывает переданные данные в файл.
 * 
 * @param Получает на вход название файла, указатели на headerы и массив пикселей.
 */
bool write_bmp(const char* filename, const BitmapFileHeader* bmfh, const BitmapInfoHeader* bmih, Rgb** data){
    FILE* file = fopen(filename, "wb");

    if(!file){
        fprintf(stderr, "Error: Не удалось открыть файл для записи выходного изображения.\n");
        return 1;
    }

    fwrite(bmfh, sizeof(BitmapFileHeader), 1, file);
    fwrite(bmih, sizeof(BitmapInfoHeader), 1, file);

    fseek(file, bmfh->pixelArrOffset, SEEK_SET);

    int row_paded = get_padding(bmih->width);

    for(int y = 0; y < bmih->height; y++){
        for(int x = 0; x < bmih->width; x++)
            fwrite(&data[y][x], sizeof(Rgb), 1, file);
        
        // Добавляеся padding для выравнивания строки кратности 4.
        unsigned char padding = 0;
        for(int i = 0; i < row_paded - bmih->width*3; i++)
            fwrite(&padding, sizeof(unsigned char), 1, file);
    }

    fclose(file);
    return 0;
}



/**
 * @brief Функция которая печатает информацию headera файла.
 * 
 * Используется в функции печатающей информацию о файле.
 * 
 * @param Получает на вход FileHeader.
 */
void print_file_header(BitmapFileHeader bmfh){
    printf("signature:          \t%x (%hu)\n", bmfh.signature, bmfh.signature);
    printf("filesize:           \t%x (%u)\n", bmfh.filesize, bmfh.filesize);
    printf("reserved1:          \t%x (%hu)\n", bmfh.reserved1, bmfh.reserved1);
    printf("reserved2:          \t%x (%hu)\n", bmfh.reserved2, bmfh.reserved2);
    printf("pixelArrOffset:     \t%x (%u)\n", bmfh.pixelArrOffset, bmfh.pixelArrOffset);
}


/**
 * @brief Функция которая печатает информацию headera информации.
 * 
 * Используется в функции печатающей информацию о файле.
 * 
 * @param Получает на вход InfoHeader.
 */
void print_info_header(BitmapInfoHeader bmih){
    printf("headerSize:         \t%x (%u)\n", bmih.headerSize, bmih.headerSize);
    printf("width:              \t%x (%u)\n", bmih.width, bmih.width);
    printf("height:             \t%x (%u)\n", bmih.height, bmih.height);
    printf("planes:             \t%x (%hu)\n", bmih.planes, bmih.planes);
    printf("bitsPerPixel:       \t%x (%hu)\n", bmih.bitsPerPixel, bmih.bitsPerPixel);
    printf("compression:        \t%x (%u)\n", bmih.compression, bmih.compression);
    printf("imageSize:          \t%x (%u)\n", bmih.imageSize, bmih.imageSize);
    printf("xpixelsPerMeter:    \t%x (%u)\n", bmih.xPixelsPerMeter, bmih.xPixelsPerMeter);
    printf("ypixelsPerMeter:    \t%x (%u)\n", bmih.yPixelsPerMeter, bmih.yPixelsPerMeter);
    printf("colorsInColorTable: \t%x (%u)\n", bmih.colorsInColorTable, bmih.colorsInColorTable);
    printf("importantColorCount:\t%x (%u)\n", bmih.importantColorCount, bmih.importantColorCount);
}



