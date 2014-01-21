#include <stdio.h>
#include <stdlib.h>
#include "animations.h"

int refresh(alt_up_pixel_buffer_dma_dev *pixel_buffer) {
    alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
    while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
    alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

    return 1;
}

int pulsing_box(alt_up_pixel_buffer_dma_dev *pixel_buffer) {
    int x0 = X_OFFSET, y0 = Y_OFFSET, x1 = WIDTH, y1 = HEIGHT, i, j, color = 0;

    for (j = 1; j < HEIGHT/2; j++) {
        for (i = 0; i < j; i++) {
            color += COLOR_INC;
            alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, x0 + i, y0 + i, x1 - i, y1 - i, 0xFFFF, 1);
        }

        refresh(pixel_buffer);
    }

    for (j = HEIGHT / 2 - 1; j >= 0; j--) {
        for (i = HEIGHT / 2; i >= j; i--) {
            alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, x0 + i, y0 + i, x1 - i, y1 - i, 0, 1);
        }

        refresh(pixel_buffer);
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
