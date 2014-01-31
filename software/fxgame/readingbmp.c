#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)

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

    fread(&bmfh->bmp_type, sizeof(uint16_t), 1, fd);
    fread(&bmfh->bmp_size, sizeof(uint32_t), 1, fd);

    printf("%lu\n", sizeof(uint16_t));
    printf("%lu\n", sizeof(uint32_t));


    printf("%x\n", bmfh->bmp_type);
    printf("%u\n", bmfh->bmp_size);

    return NULL;
}

int main() {


    return 1;
}
