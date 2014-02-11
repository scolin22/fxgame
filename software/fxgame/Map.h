#ifndef MAP_H
#define MAP_H

#include "animations.h"

#define NUM_PLAYERS 2

static const int SCREEN_WIDTH = 320;
static const int SCREEN_HEIGHT = 240;
static const int TILE_SIZE = 16;
#define NTILEX 20 //SCREEN_WIDTH / TILE_SIZE
#define NTILEY 15 //SCREEN_HEIGHT / TILE_SIZE

static const char maplayout[] = "55555555555555555555"
                                "50066666666666666005"
                                "50464646466464646405"
                                "56666666666666666665"
                                "56464646466464646465"
                                "56666666666666666665"
                                "56464646466464646465"
                                "56666666666666666665"
                                "56464646466464646465"
                                "56666666666666666665"
                                "56464646466464646465"
                                "56666666666666666665"
                                "56464646466464646465"
                                "50066666666666666005"
                                "55555555555555555555";

typedef enum {
    GRASS,
    FRUIT,
    EXPLOSION,
    CRATE,
    BLOCK,
    END
} tile_t;

typedef struct mapTile {
    char t;
    char db;
} mapTile;

mapTile** initMap ();

void destroyMap (mapTile** d);
tile_t changeTile(mapTile** d, int x, int y, tile_t tile);
tile_t checkType (mapTile** d, int x, int y);
void debugPrint (mapTile** d);
void renderMap (mapTile** d, alt_up_pixel_buffer_dma_dev *pixel_buffer);

char x_to_tx (int x);
char y_to_ty (int y);


#endif //Map_H
