#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <system.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "queue_type.h"
#include "animations.h"

int main(void) {
    printf("Hello Colin, from Nios II!\n");

    //Character Buffer
    alt_up_char_buffer_dev *char_buffer;
    char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
    alt_up_char_buffer_init(char_buffer);

    //Write some text
    alt_up_char_buffer_string(char_buffer, "WELCOME TO FRUIT EXPLOSION", 27, 29);
    alt_up_char_buffer_string(char_buffer, "THE FRUIT EXPLODING GAME", 28, 31);
    alt_up_char_buffer_string(char_buffer, "GAME COMING SOON", 32, 36);

    //Pixel Buffer
    alt_up_pixel_buffer_dma_dev *pixel_buffer;

    //Opens the pixel buffer device specified by /dev/pixel_buffer_dma
    pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");

    unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
    unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (320 * 320 * 2);

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

    //Do your thing
    Queue * x0_queue = queue_new(20);
    Queue * y0_queue = queue_new(20);
    Queue * x1_queue = queue_new(20);
    Queue * y1_queue = queue_new(20);
    // while(1) {
    //     // spinner(pixel_buffer, x0_queue, x1_queue, y0_queue, y1_queue);
    //     pulsing_box(pixel_buffer);
    // }

    infinite_spinner(pixel_buffer, x0_queue, x1_queue, y0_queue, y1_queue);
    return 0;
}
