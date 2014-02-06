#ifndef MAP_H
#define MAP_H

#include "animations.h"

#define NUM_PLAYERS 2

static const int SCREEN_WIDTH = 320;
static const int SCREEN_HEIGHT = 240;
static const int TILE_SIZE = 16;
static const int NTILEX = 20; //SCREEN_WIDTH / TILE_SIZE
static const int NTILEY = 15; //SCREEN_HEIGHT / TILE_SIZE

static const char maplayout[] = "55555555555555555555"
                                "5000006789:;<0000005"
                                "50404343403434343435"
                                "50030333330030330335"
                                "50404343430434343405"
                                "50303033000303003035"
                                "50434343430434040435"
                                "50300003333003030305"
                                "50434343433434340435"
                                "50303330330033000335"
                                "50404343433404343435"
                                "50030003003000333005"
                                "50434343433434343405"
                                "50000000000000000005"
                                "55555555555555555555";

typedef enum {
    GRASS,
    FRUIT,
    EXPLOSION,
    CRATE,
    BLOCK,
    END,
    POWERUP_FRUITS,
    POWERUP_RADIUS,
    POWERUP_KICK,
    POWERUP_THROW,
    POWERUP_INVINCIBLE,
    POWERUP_BULLDOZER
} tile_t;

typedef struct mapTile {
    char t;
    char db;
    char playerOn;
} mapTile;

extern mapTile** map;

mapTile** initMap ();

void destroyMap (mapTile** d);
tile_t changeTile(mapTile** d, int x, int y, tile_t tile);
tile_t checkType (mapTile** d, int x, int y);
void debugPrint (mapTile** d);
void renderMap (mapTile** d, alt_up_pixel_buffer_dma_dev *pixel_buffer);

char x_to_tx (int x);
char y_to_ty (int y);
void set_db(mapTile** d, int x, int y);

#endif //Map_H
