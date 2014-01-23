#ifndef MAP_H_
#define MAP_H_

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

typedef struct {
    int pos[2];
    char type;
} MapCell;



#endif
