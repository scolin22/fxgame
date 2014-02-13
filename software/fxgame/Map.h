#ifndef MAP_H
#define MAP_H

#include "animations.h"
#include "Types.h"

#define NUM_TILES 15

static const int SCREEN_WIDTH = 320;
static const int SCREEN_HEIGHT = 240;
static const int TILE_SIZE = 16;
#define NTILEX 20 //SCREEN_WIDTH / TILE_SIZE
#define NTILEY 15 //SCREEN_HEIGHT / TILE_SIZE

static const char maplayout[] = "55555555555555555555"
                                "50006666666666660005"
                                "50464646466464646405"
                                "50666666666666666605"
                                "56464646466464646465"
                                "56666666666666666665"
                                "56464646466464646465"
                                "56666666666666666665"
                                "56464646466464646465"
                                "56666666666666666665"
                                "56464646466464646465"
                                "50666666666666666605"
                                "50464646466464646405"
                                "50006666666666660005"
                                "55555555555555555555";
//6789:

static const int tileWeight[NUM_TILES] = {10,0,0,0,0,0,8,10,5,5,1,5,0,0,0};
static const int spawnRate[NUM_TILES] = {0,0,0,10,0,0,0,0,0,0,0,0,10,5,1};

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
    POWERUP_TOSS,
    POWERUP_INVINCIBLE,
    POWERUP_BULLDOZER,
    COLLECTABLE_1,
    COLLECTABLE_2,
    COLLECTABLE_3
} tile_t;

typedef struct mapTile {
    char t;
    char db;
    char playerOn;
    char owner;
    fruitType type;
    char hasPowerUp;
    char lastTile;
} mapTile;

void tilePowerUp(mapTile** d, int x, int y, int on);
void destroyMap (mapTile** d);
tile_t changeTileWithOwner(mapTile** d, int x, int y, tile_t tile, char owner, fruitType type);
tile_t changeTile(mapTile** d, int x, int y, tile_t tile);
tile_t checkType (mapTile** d, int x, int y);
void debugPrint (mapTile** d);
void renderMap (mapTile** d, alt_up_pixel_buffer_dma_dev *pixel_buffer);
void initMap (mapTile** d);
mapTile** allocateMap ();

char x_to_tx (int x);
char y_to_ty (int y);
void set_db(mapTile** d, int x, int y);

#endif //Map_H
