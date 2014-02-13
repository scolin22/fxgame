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
#include "Sound.h"

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
    Pixel* O_pixel_map;
    Pixel* OX_pixel_map;
    Pixel* B_pixel_map;
    Pixel* BX_pixel_map;
    Pixel* C_pixel_map;
    Pixel* CX_pixel_map;
    Pixel* W_pixel_map;
    Pixel* WX_pixel_map;
    Pixel* PLUS_pixel_map;
    Pixel* RAD_pixel_map;
    Pixel* KICK_pixel_map;
    Pixel* GLVE_pixel_map;
    Pixel* STAR_pixel_map;
    Pixel* BULL_pixel_map;
    Pixel* BRNZ_pixel_map;
    Pixel* SILV_pixel_map;
    Pixel* GOLD_pixel_map;

    Pixel* BUP_pixel_map;
    Pixel* BDOWN_pixel_map;
    Pixel* BRIGHT_pixel_map;
    Pixel* BLEFT_pixel_map;

    Pixel* CUP_pixel_map;
    Pixel* CDOWN_pixel_map;
    Pixel* CRIGHT_pixel_map;
    Pixel* CLEFT_pixel_map;

    Pixel* WUP_pixel_map;
    Pixel* WDOWN_pixel_map;
    Pixel* WRIGHT_pixel_map;
    Pixel* WLEFT_pixel_map;

    Pixel* OUP_pixel_map;
    Pixel* ODOWN_pixel_map;
    Pixel* ORIGHT_pixel_map;
    Pixel* OLEFT_pixel_map;

    Pixel* BCRATE_pixel_map;
    Pixel* BWALL_pixel_map;
    Pixel* BGRASS_pixel_map;

    Pixel* CCRATE_pixel_map;
    Pixel* CWALL_pixel_map;
    Pixel* CGRASS_pixel_map;

    Pixel* OCRATE_pixel_map;
    Pixel* OWALL_pixel_map;
    Pixel* OGRASS_pixel_map;

    Pixel* WCRATE_pixel_map;
    Pixel* WWALL_pixel_map;
    Pixel* WGRASS_pixel_map;
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
