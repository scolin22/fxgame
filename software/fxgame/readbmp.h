#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PS 256

#pragma pack(1)

// Note, you must disable alpha channel in the bitmap. You can do this on OSX by opening the bitmap in Preview, File->Export, Option+LClick on the File Types, Select Microsoft BMP and uncheck Alpha Channel.
typedef struct BitMapFileHeader {
    uint16_t bmp_type;
    uint32_t bmp_size;
    uint32_t bmp_reserved;
    uint32_t bmp_offset;
    uint32_t bmp_header_size;
    uint32_t bmp_width;
    uint32_t bmp_height;
    uint16_t bmp_planes;
    uint16_t bmp_bits_pixel;
    uint32_t bmp_compression;
    uint32_t bmp_pixel_data_size;
    uint32_t bmp_width_ppm;
    uint32_t bmp_height_ppm;
    uint32_t bmp_colors_used;
    uint32_t bmp_imp_colors;
} BitmapFileHeader;

typedef struct Pixel{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;

unsigned char* pixel_data(char* filename, BitmapFileHeader* bmfh);

Pixel* convert_24_to_16(Pixel* pixel_map_24);

#endif
