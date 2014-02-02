#ifndef MAP_H
#define MAP_H

#include "Fruits.h"
#include "animations.h"

#define NUM_PLAYERS 2

static const int SCREEN_WIDTH = 320;
static const int SCREEN_HEIGHT = 240;
static const int TILE_SIZE = 16;
static const int NTILEX = 20; //SCREEN_WIDTH / TILE_SIZE
static const int NTILEY = 15; //SCREEN_HEIGHT / TILE_SIZE

static const char maplayout[] = "44444444444444444444"
								"40000000000000000004"
								"40404040400404040404"
								"40000000000000000004"
								"40404040400404040404"
								"40000000000000000004"
								"40404040400404040404"
								"40000000000000000004"
								"40404040400404040404"
								"40000000000000000004"
								"40404040400404040404"
								"40000000000000000004"
								"40404040400404040404"
								"40000000000000000004"
								"44444444444444444444";

typedef enum {
	GRASS,
	FRUIT,
	EXPLOSION,
	CRATE,
	BLOCK,
	END
} tile_t;

char** initMap ();

void destroyMap (char** d);
tile_t changeTile(char** d, int x, int y, tile_t tile);
tile_t checkType (char** d, int x, int y);
void debugPrint (char** d);
void renderMap (char** d, alt_up_pixel_buffer_dma_dev *pixel_buffer);

char x_to_tx (int x);
char y_to_ty (int y);


#endif //Map_H
