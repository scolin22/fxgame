#include <stdio.h>
#include <stdlib.h>

#define PS 16

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

Pixel pixel_map[PS][PS];

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

int main(int argc, char *argv[]) {
    BitmapFileHeader bmfh;

    unsigned char* bmp_data = pixel_data(argv[1], &bmfh);

    // In a traditional 24-bit uncompressed RGB color mode, we don't use color lookup tables nor "colors available" and "colors used" data fields are used. Each pixel is 8 bits of blue, green, then red. From Left->Down
    int i, j, k = 0;
    for (i = 0; i < PS; i++) {
        for (j = 0; j < PS; j++)
        {
            pixel_map[j][i].blue = bmp_data[k++];
            pixel_map[j][i].green = bmp_data[k++];
            pixel_map[j][i].red = bmp_data[k++];
        }
    }

    for (i = 0; i < PS; i++) {
        for (j = 0; j < PS; j++)
        {
            printf("%02X%02X%02X ", pixel_map[j][i].blue, pixel_map[j][i].green, pixel_map[j][i].red);
        }
        printf("\n");
    }

    return 1;
}
