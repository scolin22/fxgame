#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "queue_type.h"

#define HEIGHT 240
#define WIDTH 320
#define X_OFFSET 0 //6
#define Y_OFFSET 0
#define SLEEP 15625
#define COLOR_MAX 65535
#define COLOR_INC 546

int pulsing_box(alt_up_pixel_buffer_dma_dev *pixel_buffer);

int spinner(alt_up_pixel_buffer_dma_dev *pixel_buffer, Queue * x0_queue, Queue * x1_queue, Queue * y0_queue, Queue * y1_queue);

int infinite_spinner(alt_up_pixel_buffer_dma_dev *pixel_buffer, Queue * x0_queue, Queue * x1_queue, Queue * y0_queue, Queue * y1_queue);

int pulsing_box(alt_up_pixel_buffer_dma_dev *pixel_buffer) {
    int x0 = X_OFFSET, y0 = Y_OFFSET, x1 = WIDTH, y1 = HEIGHT, i, color = 0;

    for (i = 0; i < HEIGHT / 2; i++) {
        color += COLOR_INC;
        alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, x0 + i, y0 + i, x1 - i, y1 - i, color, 0);
        usleep(SLEEP/2);
    }
    for (i = HEIGHT / 2; i >= 0; i--) {
        color = 0x0000;
        alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, x0 + i, y0 + i, x1 - i, y1 - i, color, 0);
        usleep(SLEEP/2);
    }

    return 0;
}

int spinner(alt_up_pixel_buffer_dma_dev *pixel_buffer, Queue * x0_queue, Queue * x1_queue, Queue * y0_queue, Queue * y1_queue) {
    // int x0 = -40, y0 = 120, x1 = 360, y1 = 120;
    float theta = 0.0, r = 100.0, inc = 0.01745, pi = 3.14159;
    int x0, y0, x1, y1, i, color = 0xFFFF, delay = 19, delay_count = delay;

    while (theta < pi) {
        //if (theta > pi) theta = 0.0;

        //I don't know the cost of calculation
        x0 = (int)(r * cos(theta)) + WIDTH / 2;
        y0 = (int)(r * sin(theta)) + HEIGHT / 2;
        x1 = (int)(r * cos(theta + pi)) + WIDTH / 2;
        y1 = (int)(r * sin(theta + pi)) + HEIGHT / 2;

        theta += inc;

        enqueue(x0_queue, x0);
        enqueue(y0_queue, y0);
        enqueue(x1_queue, x1);
        enqueue(y1_queue, y1);

        color += COLOR_INC;
        alt_up_pixel_buffer_dma_draw_line(pixel_buffer, x0, y0, x1, y1, color, 0);

        if (delay_count == 0) {
            x0 = dequeue(x0_queue);
            y0 = dequeue(y0_queue);
            x1 = dequeue(x1_queue);
            y1 = dequeue(y1_queue);

            alt_up_pixel_buffer_dma_draw_line(pixel_buffer, x0, y0, x1, y1, 0, 0);
        }
        else
            delay_count--;

        usleep(SLEEP);
    }

    // Clears the old lines
    while (!is_empty(x0_queue)) {
        x0 = dequeue(x0_queue);
        y0 = dequeue(y0_queue);
        x1 = dequeue(x1_queue);
        y1 = dequeue(y1_queue);

        alt_up_pixel_buffer_dma_draw_line(pixel_buffer, x0, y0, x1, y1, 0, 0);
    }

    return 0;
}

int infinite_spinner(alt_up_pixel_buffer_dma_dev *pixel_buffer, Queue * x0_queue, Queue * x1_queue, Queue * y0_queue, Queue * y1_queue) {
    // int x0 = -40, y0 = 120, x1 = 360, y1 = 120;
    float theta = 0.0, r = 100.0, inc = 0.01745, pi = 3.14159;
    int x0, y0, x1, y1, i, color = 0xFFFF, delay = 19, delay_count = delay;

    while (1) {
        if (theta > pi) theta = 0.0;

        //I don't know the cost of calculation
        x0 = (int)(r * cos(theta)) + WIDTH / 2;
        y0 = (int)(r * sin(theta)) + HEIGHT / 2;
        x1 = (int)(r * cos(theta + pi)) + WIDTH / 2;
        y1 = (int)(r * sin(theta + pi)) + HEIGHT / 2;

        theta += inc;

        enqueue(x0_queue, x0);
        enqueue(y0_queue, y0);
        enqueue(x1_queue, x1);
        enqueue(y1_queue, y1);

        color += COLOR_INC;
        alt_up_pixel_buffer_dma_draw_line(pixel_buffer, x0, y0, x1, y1, color, 0);

        if (delay_count == 0) {
            x0 = dequeue(x0_queue);
            y0 = dequeue(y0_queue);
            x1 = dequeue(x1_queue);
            y1 = dequeue(y1_queue);

            alt_up_pixel_buffer_dma_draw_line(pixel_buffer, x0, y0, x1, y1, 0, 0);
        }
        else
            delay_count--;

        usleep((int)SLEEP);
    }
    return 0;
}

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

    //Set the background buffer address - Although we don't use the background, they only provide a function to change the background buffer address, so we must set that, and then swap it to the foreground
    //Changes the back buffer's start address
    alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, PIXEL_BUFFER_BASE);

    //Swap background and foreground buffers
    alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);

    //Wait for the swap to complete
    while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

    //Clear the screen
    alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

    //Do your thing
    Queue * x0_queue = queue_new(20);
    Queue * y0_queue = queue_new(20);
    Queue * x1_queue = queue_new(20);
    Queue * y1_queue = queue_new(20);
    while(1) {
        // spinner(pixel_buffer, x0_queue, x1_queue, y0_queue, y1_queue);
        pulsing_box(pixel_buffer);
    }

    // infinite_spinner(pixel_buffer, x0_queue, x1_queue, y0_queue, y1_queue);
    return 0;
}
