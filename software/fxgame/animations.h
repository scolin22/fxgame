#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <system.h>
#include <bmpfile.h>
#include "queue_type.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"

#define HEIGHT 240
#define WIDTH 320
#define X_OFFSET 0 //6
#define Y_OFFSET 0
#define SLEEP 15625
#define COLOR_MAX 65535
#define COLOR_INC 546

alt_up_char_buffer_dev *init_char_stuff(char *location);

alt_up_pixel_buffer_dma_dev *init_pixel_stuff(char *location);

int refresh(alt_up_pixel_buffer_dma_dev *pixel_buffer);

// int draw_bmp(alt_up_pixel_buffer_dma_dev *pixel_buffer, int* bitstream);

#endif
