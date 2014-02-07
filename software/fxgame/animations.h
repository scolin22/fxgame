#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <system.h>
#include <string.h>

#include "readbmp.h"
#include "sdcard.h"
#include "queue_type.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "io.h"

#define HEIGHT 240
#define WIDTH 320
#define X_OFFSET 0 //6
#define Y_OFFSET 0
#define SLEEP 15625
#define COLOR_MAX 65535
#define COLOR_INC 546

typedef struct Pixel_Map{
    Pixel* TEST1_pixel_map;
    Pixel* GRASS_pixel_map;
    Pixel* FB_pixel_map;
    Pixel* VOLT_pixel_map;
    Pixel* PIKA_pixel_map;
    Pixel* WALL_pixel_map;
    Pixel* BOMB_pixel_map;
    Pixel* EXPL_pixel_map;
    Pixel* CRATE_pixel_map;
} Pixel_Map;

extern Pixel_Map* booted_bmps;

alt_up_char_buffer_dev *init_char_stuff(char *location);

alt_up_pixel_buffer_dma_dev *init_pixel_stuff(char *location);

int refresh(alt_up_pixel_buffer_dma_dev *pixel_buffer);

int draw_screen_from_pixel_map_16(alt_up_pixel_buffer_dma_dev *pixel_buffer, Pixel* pixel_map_16, int x0, int y0);

int draw_screen_from_bmp(alt_up_pixel_buffer_dma_dev *pixel_buffer, Pixel_Map* booted_bmps, int filename, int x, int y);

int boot_bmps(Pixel_Map* booted_bmps);

int draw_pixel_fast(alt_up_pixel_buffer_dma_dev *pixel_buffer,
        unsigned int color, unsigned int x, unsigned int y);

#endif
