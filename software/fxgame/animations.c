#include <string.h>
#include "animations.h"
#include "readbmp.h"

Pixel_Map* booted_bmps;

//alt_up_char_buffer_dev *char_buffer = init_char_stuff("/dev/char_drawer");
alt_up_char_buffer_dev *init_char_stuff(char *location) {
    //Character Buffer
    alt_up_char_buffer_dev *char_buffer;
    char_buffer = alt_up_char_buffer_open_dev(location);
    alt_up_char_buffer_init(char_buffer);

    return char_buffer;
}

//alt_up_pixel_buffer_dma_dev *pixel_buffer = init_pixel_stuff("/dev/pixel_buffer_dma");
alt_up_pixel_buffer_dma_dev *init_pixel_stuff(char *location){
    //Pixel Buffer
    alt_up_pixel_buffer_dma_dev *pixel_buffer;

    //Opens the pixel buffer device specified by /dev/pixel_buffer_dma
    pixel_buffer = alt_up_pixel_buffer_dma_open_dev(location);

    unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
    unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + PIXEL_BUFFER_SPAN / 2;

    //Change the 1st buffer's start address
    alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr1);

    //Swap background and foreground buffers
    alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);

    //Wait for the swap to complete
    while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

    //Change the 1st buffer's start address
    alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr2);

    //Clear the screen
    alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
    alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

    return pixel_buffer;
}


int refresh(alt_up_pixel_buffer_dma_dev *pixel_buffer) {
    alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
    while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
    usleep(33333);
    return 1;
}

int draw_screen_from_pixel_map_16(alt_up_pixel_buffer_dma_dev *pixel_buffer, Pixel* pixel_map_16, int x0, int y0) {
    int i, j = 0, x, y;
    uint16_t color;
    for (i = 0; i < PS; i++) {
        x = i % 16;
        // Go down one line, except if it's the first line
        if (i % 16 == 0 && i != 0){
            j++;
        }
        y = j;

        color = pixel_map_16[i].red << 11 | pixel_map_16[i].green << 5 | pixel_map_16[i].blue;
        if (color != 0x7C0) {
            alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, x + x0, y + y0, x + x0, y + y0, color, 1);
        }
    }
    return 1;
}

int draw_screen_from_bmp(alt_up_pixel_buffer_dma_dev *pixel_buffer, Pixel_Map* booted_bmps, int filename, int x, int y) {
    //printf("READING BMP\n");
    Pixel* pixel_map_16 = NULL;
    if (filename == 0) {
        pixel_map_16 = booted_bmps->TEST1_pixel_map;
    } else if (filename == 1) {
        pixel_map_16 = booted_bmps->GRASS_pixel_map;
    } else if (filename == 2) {
        pixel_map_16 = booted_bmps->FB_pixel_map;
    } else if (filename == 3) {
        pixel_map_16 = booted_bmps->VOLT_pixel_map;
    } else if (filename == 4) {
        pixel_map_16 = booted_bmps->PIKA_pixel_map;
    } else if (filename == 5) {
        pixel_map_16 = booted_bmps->WALL_pixel_map;
    }

    //printf("Finished Converting 24 to 16\n");
    draw_screen_from_pixel_map_16(pixel_buffer, pixel_map_16, x, y);

    return 1;
}

int boot_bmps(Pixel_Map* booted_bmps) {
    booted_bmps->TEST1_pixel_map = init_pixel_map_16_from_bmp("TEST1.BMP");
    booted_bmps->GRASS_pixel_map = init_pixel_map_16_from_bmp("GRASS.BMP");
    booted_bmps->FB_pixel_map = init_pixel_map_16_from_bmp("FB.BMP");
    booted_bmps->VOLT_pixel_map = init_pixel_map_16_from_bmp("VOLT.BMP");
    booted_bmps->PIKA_pixel_map = init_pixel_map_16_from_bmp("PIKA.BMP");
    booted_bmps->WALL_pixel_map = init_pixel_map_16_from_bmp("WALL.BMP");

    return booted_bmps;
}
