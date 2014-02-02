#include "animations.h"
#include "readbmp.h"

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
            printf("\n");
        }
        y = j;
        //printf("%02X%02X%02X.", pixel_map_16[i].red, pixel_map_16[i].green, pixel_map_16[i].blue);
        color = pixel_map_16[i].red << 11 | pixel_map_16[i].green << 5 | pixel_map_16[i].blue;
        printf("D%X X%d Y%d", color, x + x0, y + y0);
        alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, x + x0, y + y0, x + x0, y + y0, color, 0);
    }
    return 1;
}
