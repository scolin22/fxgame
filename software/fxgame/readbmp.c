#include "readbmp.h"

unsigned char* pixel_data(char* filename, BitmapFileHeader* bmfh) {
    FILE* fd;
    printf("Opening %s\n", filename);
    fd = fopen(filename, "rb");

    if (fd == NULL) {
        printf("FAILED TO OPEN\n");
        return NULL;
    }

    fread(bmfh, sizeof(BitmapFileHeader), 1, fd);
    bmfh->bmp_pixel_data_size = bmfh->bmp_size - bmfh->bmp_offset;
    unsigned char *bmp_data = (unsigned char*) malloc(bmfh->bmp_pixel_data_size);
    fread(bmp_data, bmfh->bmp_pixel_data_size, 1, fd);

    return bmp_data;
}

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
