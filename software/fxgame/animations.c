#include "animations.h"

alt_up_char_buffer_dev *init_char_stuff(char *location) {
    //Character Buffer
    alt_up_char_buffer_dev *char_buffer;
    char_buffer = alt_up_char_buffer_open_dev(location);
    alt_up_char_buffer_init(char_buffer);

    return char_buffer;
}

alt_up_pixel_buffer_dma_dev *init_pixel_stuff(char *location){
    //Pixel Buffer
    alt_up_pixel_buffer_dma_dev *pixel_buffer;

    //Opens the pixel buffer device specified by /dev/pixel_buffer_dma
    pixel_buffer = alt_up_pixel_buffer_dma_open_dev(location);

    unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
    unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + PIXEL_BUFFER_SPAN;

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

    return 1;
}

int draw_pixel(alt_up_pixel_buffer_dma_dev *pixel_buffer) {
    return 1;
}

int draw_bmp(alt_up_pixel_buffer_dma_dev *pixel_buffer, int* bitstream) {
    return 1;
}
