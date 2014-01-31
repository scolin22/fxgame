#include <stdio.h>
#include <stdlib.h>

#define PS 256

#pragma pack(push, 1)

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


#pragma pack(pop)

typedef struct Pixel{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;

unsigned char* pixel_data(char* filename, BitmapFileHeader* bmfh) {
    FILE* fd;

    printf("Opening %s\n", filename);

    fd = fopen(filename, "rb");

    if (fd == NULL) {
        printf("FAILED TO OPEN\n");
        return NULL;
    }

    // Helps to test if we're doing everything correctly
    // long verify_size;
    // fseek(fd, 0, SEEK_END);
    // verify_size = ftell(fd);
    // rewind(fd);
    // printf("verify size %ld\n", verify_size);

    fread(bmfh, sizeof(BitmapFileHeader), 1, fd);

    // printf("%lu\n", sizeof(BitmapFileHeader));
    // printf("%x\n", bmfh->bmp_type);
    // printf("%u\n", bmfh->bmp_size);
    // printf("%u\n", bmfh->bmp_reserved);
    // printf("%u\n", bmfh->bmp_offset);
    // printf("%u\n", bmfh->bmp_header_size);
    // printf("%u\n", bmfh->bmp_width);
    // printf("%u\n", bmfh->bmp_height);
    // printf("%u\n", bmfh->bmp_planes);
    // printf("%u\n", bmfh->bmp_bits_pixel);
    // printf("%u\n", bmfh->bmp_compression);
    // printf("%u\n", bmfh->bmp_pixel_data_size);
    // printf("%u\n", bmfh->bmp_width_ppm);
    // printf("%u\n", bmfh->bmp_height_ppm);
    // printf("%u\n", bmfh->bmp_colors_used);
    // printf("%u\n", bmfh->bmp_imp_colors);

    bmfh->bmp_pixel_data_size = bmfh->bmp_size - bmfh->bmp_offset;

    unsigned char *bmp_data = (unsigned char*) malloc(bmfh->bmp_pixel_data_size);
    fread(bmp_data, bmfh->bmp_pixel_data_size, 1, fd);

    return bmp_data;
}

// Pixel **alloc_array(int rows, int columns)
// {
//     int i;
//     int j;
//     Pixel **PixelMap = (Pixel**) (malloc(rows * sizeof(Pixel *)));
//     Pixel **PixelMapStart = PixelMap;
//     Pixel *currentrow;

//     for ( i = 0; i < rows; i++ ){
//         *(PixelMap + i) =  (malloc(columns * sizeof(Pixel)));
//     }

//     // Correct way to access but incorrect type
//     // for (j = 0; j < rows; j++) {
//     //     currentrow = *(PixelMap + j);
//     //     for ( i = 0; i < columns; i++ ) {
//     //         *(currentrow + i) = i;
//     //         printf("%d\n", *(currentrow+i));
//     //     }
//     // }
//     return PixelMap;
// }

Pixel* convert_24_to_16(Pixel* pixel_map_24) {
    Pixel* pixel_map_16 = (Pixel*) malloc(sizeof(Pixel) * PS);

    int i;
    for (i = 0; i < PS; i++) {
        pixel_map_16[i].blue = pixel_map_24[i].blue / 8;
        pixel_map_16[i].green = pixel_map_24[i].green / 4;
        pixel_map_16[i].red = pixel_map_24[i].red / 8;
    }

    return pixel_map_16;
}

int main(int argc, char *argv[]) {
    BitmapFileHeader bmfh;

    unsigned char* bmp_data = pixel_data(argv[1], &bmfh);

    Pixel pixel_map[PS];

    // In a traditional 24-bit uncompressed RGB color mode, we don't use color lookup tables nor "colors available" and "colors used" data fields are used. Each pixel is 8 bits of blue, green, then red. From Left->Down
    int i, k = 0;
    for (i = 0; i < PS; i++)
    {
        pixel_map[i].blue = bmp_data[k++];
        pixel_map[i].green = bmp_data[k++];
        pixel_map[i].red = bmp_data[k++];
    }

    for (i = 0; i < PS; i++)
    {
        if (i % 16 == 0 ) printf("\n");
        printf("%02X%02X%02X ", pixel_map[i].blue, pixel_map[i].green, pixel_map[i].red);
    }
    printf("\n");

    Pixel* pixel_map_16 = convert_24_to_16(pixel_map);

    for (i = 0; i < PS; i++)
    {
        if (i % 16 == 0 ) printf("\n");
        printf("%02X%02X%02X ", pixel_map_16[i].blue, pixel_map_16[i].green, pixel_map_16[i].red);
    }
    printf("\n");

    return 1;
}
