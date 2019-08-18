//
// Created by alpharius on 18.08.19.
//

#ifndef FURIER_TRANSFORM_GRAPH_IN_BPM_H
#define FURIER_TRANSFORM_GRAPH_IN_BPM_H

struct rgb_data {
    float r, g, b;
};

//
/*
 *  функция записывающая значения пикселей в соответствующий файл
 *  принимает:
 *      file_name - имя файла
 *      width, height - линейные размеры выходного изображения
 *      dpi - число точек на дюйм
 *      rgb_data - указатель на массив пикселей
 *
 *  результат работы:
 *      bpm изображение, соответствующее переданным битам
 *
 *  комментарий:
 *      код этой функции не мой, позаимствовал
 *      http://ricardolovelace.com/creating-bitmap-images-with-c-on-windows.html
 */
void save_bitmap(const char *file_name, int width, int height, int dpi, struct rgb_data *pixel_data);


void draw_graphic(const char *input_file,int size);
#endif //FURIER_TRANSFORM_GRAPH_IN_BPM_H
