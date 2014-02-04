
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

    FruitCtrl* fruitCtrl = malloc(sizeof(FruitCtrl));
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
    p1->fruitKey = 'Z';
    p1->lives = 10;

    Player* p2 = (Player*)malloc(sizeof(Player));

    p2->posX = 16+TILE_SIZE*10;
    p2->posY = 16+TILE_SIZE*10;
    p2->height = TILE_SIZE-2;
    p2->width = TILE_SIZE-2;
    p2->dropBomb = 0;
    p2->respawnTime = 0;
    p2->id = 1;
    p2->leftKey = 'J';
    p2->rightKey = 'L';
    p2->upKey = 'I';
    p2->downKey = 'K';
    p2->fruitKey = 'M';
    p2->lives = 10;

    Score* score = (Score*)malloc(sizeof(Score));

    char key;

    printf("Initializing Setup \n");
    kbd_init();
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
        handleEvents(p1, IORD(switches, 0));

        move(p1, map, fruitCtrl);

        handleEvents(p2, IORD(switches, 0));

        move(p2, map, fruitCtrl);

        updateFruits(fruitCtrl);

        updatePlayer(p1);
        updatePlayer(p2);

        score->scores[0] = p1->lives;
        score->scores[1] = p2->lives;

        renderMap(map, pixel_buffer);
        renderPlayer (p1, pixel_buffer);
        renderPlayer (p2, pixel_buffer);
        renderScore (score, char_buffer);
        refresh(pixel_buffer);
    }
    destroyMap(map);
    return 0;
}
