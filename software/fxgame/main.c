
#include <stdio.h>
#include <unistd.h>
#include <system.h>
#include <io.h>
#include <time.h>
#include <assert.h>

#include "Map.h"
#include "Player.h"
#include "Fruits.h"
#include "Score.h"
#include "animations.h"

#define switches (volatile char *) 0x0004430
#define leds (char *) 0x0004420

#define FPS 2.0
#define TICKS_PER_FRAME (1000.0 / FPS)
#define PS 256

int main() {
    mapTile** map = initMap();
    //initialize other stuff such as vga, sd card, etc.
    alt_up_pixel_buffer_dma_dev *pixel_buffer = init_pixel_stuff("/dev/pixel_buffer_dma");
    alt_up_char_buffer_dev *char_buffer = init_char_stuff("/dev/char_drawer");

    FruitCtrl** fruitCtrl = malloc(sizeof(FruitCtrl));
    initFruits(fruitCtrl,map);

    Player* p1 = (Player*)malloc(sizeof(Player));

    p1->posX = 16;
    p1->posY = 16;
    p1->height = TILE_SIZE-2;
    p1->width = TILE_SIZE-2;
    p1->respawnTime = 0;
    p1->dropBomb = 0;
    p1->id = 0;
    p1->leftKey = 'A';
    p1->rightKey = 'D';
    p1->upKey = 'W';
    p1->downKey = 'S';
    p1->fruitKey = 'Q';
    p1->score = 0;
    p1->fruitCtrl = fruitCtrl;
    p1->velX = 0;
    p1->velY = 0;
    p1->dir = left;
    p1->pwrUps = 0;
    p1->map = map;

    Player* p2 = (Player*)malloc(sizeof(Player));

    p2->posX = 16+TILE_SIZE*17;
    p2->posY = 16+TILE_SIZE*12;
    p2->height = TILE_SIZE-2;
    p2->width = TILE_SIZE-2;
    p2->dropBomb = 0;
    p2->respawnTime = 0;
    p2->id = 1;
    p2->leftKey = '4';
    p2->rightKey = '6';
    p2->upKey = '8';
    p2->downKey = '5';
    p2->fruitKey = '7';
    p2->score = 0;
    p2->fruitCtrl = fruitCtrl;
    p2->velX = 0;
    p2->velY = 0;
    p2->dir = left;
    p2->pwrUps = 0;
    p2->map = map;

    Score* score = (Score*)malloc(sizeof(Score));

    players->list[p1->id] = p1;
    players->list[p2->id] = p2;

    char key;

    printf("Initializing Setup \n");
    kbd_init(players);
    printf("Ready for key press: \n");

    //Keep this here -Colin
   int connected = 0;
   while (connected == 0) {
       initSD(&connected);
   }

   //This is booting bmps -Colin
    booted_bmps = (Pixel_Map*) malloc(sizeof(Pixel_Map));
    booted_bmps = boot_bmps(booted_bmps);

    printf("BOOTED IMAGES\n");

    renderMap(map, pixel_buffer);
    refresh(pixel_buffer);
    renderMap(map, pixel_buffer);
    refresh(pixel_buffer);
    while (1) {
    	handleEvents(p1);
    	handleEvents(p2);
        updateFruits(fruitCtrl);
        updatePlayer(p1);
        updatePlayer(p2);
        score->scores[0] = p1->score;
        score->scores[1] = p2->score;
        renderMap(map, pixel_buffer);
        renderPlayer (p1, pixel_buffer);
        renderPlayer (p2, pixel_buffer);
        renderScore (score, char_buffer);
        refresh(pixel_buffer);
    }
    destroyMap(map);
    return 0;
}
