//
// Created by alpharius on 15.08.19.
//

#include "graph_in_bpm.h"

#include <bits/types/FILE.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


void save_bitmap(const char *file_name, int width, int height, int dpi, struct rgb_data *pixel_data){
    // код полностью не мой
    // create a file object that we will use to write our image
    FILE *image;
    // we want to know how many pixels to reserve
    int image_size = width * height;
    // a byte is 4 bits but we are creating a 24 bit image so we can represent a pixel with 3
    // our final file size of our image is the width * height * 4 + size of bitmap header
    int file_size = 54 + 4 * image_size;
    // pixels per meter https://www.wikiwand.com/en/Dots_per_inch
    int ppm = dpi * 39.375;

    // bitmap file header (14 bytes)
    // we could be savages and just create 2 array but since this is for learning lets
    // use structs so it can be parsed by someone without having to refer to the spec

    // since we have a non-natural set of bytes, we must explicitly tell the
    // compiler to not pad anything, on gcc the attribute alone doesn't work so
    // a nifty trick is if we declare the smallest data type last the compiler
    // *might* ignore padding, in some cases we can use a pragma or gcc's
    // __attribute__((__packed__)) when declaring the struct
    // we do this so we can have an accurate sizeof() which should be 14, however
    // this won't work here since we need to order the bytes as they are written
    struct bitmap_file_header {
        unsigned char   bitmap_type[2];     // 2 bytes
        int             file_size;          // 4 bytes
        short           reserved1;          // 2 bytes
        short           reserved2;          // 2 bytes
        unsigned int    offset_bits;        // 4 bytes
    } bfh;

    // bitmap image header (40 bytes)
    struct bitmap_image_header {
        unsigned int    size_header;        // 4 bytes
        unsigned int    width;              // 4 bytes
        unsigned int    height;             // 4 bytes
        short int       planes;             // 2 bytes
        short int       bit_count;          // 2 bytes
        unsigned int    compression;        // 4 bytes
        unsigned int    image_size;         // 4 bytes
        unsigned int    ppm_x;              // 4 bytes
        unsigned int    ppm_y;              // 4 bytes
        unsigned int    clr_used;           // 4 bytes
        unsigned int    clr_important;      // 4 bytes
    } bih;

    // if you are on Windows you can include <windows.h>
    // and make use of the BITMAPFILEHEADER and BITMAPINFOHEADER structs

    memcpy(&bfh.bitmap_type, "BM", 2);
    bfh.file_size       = file_size;
    bfh.reserved1       = 0;
    bfh.reserved2       = 0;
    bfh.offset_bits     = 0;

    bih.size_header     = sizeof(bih);
    bih.width           = width;
    bih.height          = height;
    bih.planes          = 1;
    bih.bit_count       = 24;
    bih.compression     = 0;
    bih.image_size      = file_size;
    bih.ppm_x           = ppm;
    bih.ppm_y           = ppm;
    bih.clr_used        = 0;
    bih.clr_important   = 0;

    image = fopen(file_name, "wb");

    // compiler woes so we will just use the constant 14 we know we have
    fwrite(&bfh, 1, 14, image);
    fwrite(&bih, 1, sizeof(bih), image);

    // write out pixel data, one last important this to know is the ordering is backwards
    // we have to go BGR as opposed to RGB
    for (int i = 0; i < image_size; i++) {
    struct rgb_data BGR = pixel_data[i];

    float red   = (BGR.r);
    float green = (BGR.g);
    float blue  = (BGR.b);

    // if you don't follow BGR image will be flipped!
    unsigned char color[3] = {
            blue, green, red
    };

    fwrite(color, 1, sizeof(color), image);
    }

    fclose(image);
}

#define TEST
#ifdef TEST
int main(void){
    int width  = 400,
            height = 400,
            dpi = 9;
    srand(time(NULL));
    struct rgb_data *pixels = (struct rgb_data*) malloc(width*height*sizeof(struct rgb_data));//new rgb_data[width * height];

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int a = y * width + x;

            if ((x > 50 && x < 350) && (y > 50 && y < 350)) {
                pixels[a].r = rand()%255;
                pixels[a].g = rand()%255;
                pixels[a].b = rand()%255;
            } else {
                pixels[a].r = 55;
                pixels[a].g = 55;
                pixels[a].b = 55;
            }
        }
    }

    save_bitmap("name.bmp", width, height, dpi,pixels);
}

#endif // TEST