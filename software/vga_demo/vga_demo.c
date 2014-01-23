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

    alt_up_char_buffer_dev *char_buffer = init_char_stuff("/dev/char_drawer");

    //Write some text
    alt_up_char_buffer_string(char_buffer, "WELCOME TO FRUIT EXPLOSION", 27, 29);
    alt_up_char_buffer_string(char_buffer, "THE FRUIT EXPLODING GAME", 28, 31);
    alt_up_char_buffer_string(char_buffer, "GAME COMING SOON", 32, 36);
    alt_up_char_buffer_string(char_buffer, "BY BYRON", 36, 38);

    alt_up_pixel_buffer_dma_dev *pixel_buffer = init_pixel_stuff("/dev/pixel_buffer_dma");

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
