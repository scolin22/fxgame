#include "readbmp.h"

Pixel* init_pixel_map_16_from_bmp(char* filename) {
    BitmapFileHeader* bmfh = (BitmapFileHeader*)malloc(8*sizeof(BitmapFileHeader));

    uint8_t *bmp_data = (uint8_t*) malloc(bmfh->bmp_pixel_data_size);
    unsigned char* bmp_data_ret = pixel_data(filename, bmfh, bmp_data);

    Pixel pixel_map[PS];

    int i, k = 0;
    for (i = 0; i < PS; i++)
    {
        pixel_map[i].blue = bmp_data_ret[k++];
        pixel_map[i].green = bmp_data_ret[k++];
        pixel_map[i].red = bmp_data_ret[k++];
    }

    Pixel* pixel_map_16 = convert_24_to_16(pixel_map);

    return pixel_map_16;
}

unsigned char* pixel_data(char* filename, BitmapFileHeader* bmfh, uint8_t *bmp_data) {
    int connected = 0;
    while (connected == 0) {
        initSD(&connected);
    }

    printf("COLIN'S DATA\n");

    uint8_t * result = (uint8_t*)malloc(54*sizeof(uint8_t));

    FILE *fd;
    fd = alt_up_sd_card_fopen("TEST1.BMP", 0);
    printf("%d\n", fd);
    readFileBytes(filename, 54, result, fd);

    int index;
    for(index = 0; index < 54; index++) {
        printf("%02X", result[index]);
    }
    printf("\n");

    assign_header_data(result, bmfh);

    bmfh->bmp_pixel_data_size = bmfh->bmp_size - bmfh->bmp_offset;
    printf("bmfh->bmp_pixel_data_size %08x\n", bmfh->bmp_pixel_data_size);

    readFileBytes(filename, bmfh->bmp_pixel_data_size, bmp_data, fd);

    if (alt_up_sd_card_fclose(fd) == 0) printf("NOTCLOSED\n");
    else printf("CLOSED\n");

    printf("Finished reading pixel data\n");

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

void assign_header_data(char* result, BitmapFileHeader* bmfh) {
    bmfh->bmp_type = result[0] << 8 | result[1];
    bmfh->bmp_size = result[2] << 24 | result[3] << 16 | result[4] << 8 | result[5];
    bmfh->bmp_reserved = result[6] << 24 | result[7] << 16 | result[8] << 8 | result[9];
    bmfh->bmp_offset = result[10] << 24 | result[11] << 16 | result[12] << 8 | result[13];
    bmfh->bmp_header_size = result[14] << 24 | result[15] << 16 | result[16] << 8 | result[17];
    bmfh->bmp_width = result[18] << 24 | result[19] << 16 | result[20] << 8 | result[21];
    bmfh->bmp_height = result[22] << 24 | result[23] << 16 | result[24] << 8 | result[25];
    bmfh->bmp_planes = result[26] << 8 | result[27];
    bmfh->bmp_bits_pixel = result[28] << 8 | result[29];
    bmfh->bmp_compression = result[30] << 24 | result[31] << 16 | result[32] << 8 | result[33];
    bmfh->bmp_pixel_data_size = result[34] << 24 | result[35] << 16 | result[36] << 8 | result[37];
    bmfh->bmp_width_ppm = result[38] << 24 | result[39] << 16 | result[40] << 8 | result[41];
    bmfh->bmp_height_ppm = result[42] << 24 | result[43] << 16 | result[44] << 8 | result[45];
    bmfh->bmp_colors_used = result[46] << 24 | result[47] << 16 | result[48] << 8 | result[49];
    bmfh->bmp_imp_colors = result[50] << 24 | result[51] << 16 | result[52] << 8 | result[53];
    // printf("bmfh->bmp_type %04x\n", bmfh->bmp_type);
    // printf("bmfh->bmp_size %08x\n", bmfh->bmp_size);
    // printf("bmfh->bmp_reserved %08x\n", bmfh->bmp_reserved);
    // printf("bmfh->bmp_offset %08x\n", bmfh->bmp_offset);
    // printf("bmfh->bmp_header_size %08x\n", bmfh->bmp_header_size);
    // printf("bmfh->bmp_width %08x\n", bmfh->bmp_width);
    // printf("bmfh->bmp_height %08x\n", bmfh->bmp_height);
    // printf("bmfh->bmp_planes %04x\n", bmfh->bmp_planes);
    // printf("bmfh->bmp_bits_pixel %04x\n", bmfh->bmp_bits_pixel);
    // printf("bmfh->bmp_compression %08x\n", bmfh->bmp_compression);
    // printf("bmfh->bmp_pixel_data_size %08x\n", bmfh->bmp_pixel_data_size);
    // printf("bmfh->bmp_width_ppm %08x\n", bmfh->bmp_width_ppm);
    // printf("bmfh->bmp_height_ppm %08x\n", bmfh->bmp_height_ppm);
    // printf("bmfh->bmp_colors_used %08x\n", bmfh->bmp_colors_used);
    // printf("bmfh->bmp_imp_colors %08x\n", bmfh->bmp_imp_colors);
}
