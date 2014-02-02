
#include <stdio.h>
#include <unistd.h>
#include <system.h>
#include <io.h>
#include <time.h>

#include "Map.h"
#include "Player.h"
#include "animations.h"

#define switches (volatile char *) 0x0004430
#define leds (char *) 0x0004420

#define FPS 2.0
#define TICKS_PER_FRAME (1000.0 / FPS)
#define PS 256

int main()
{
    char** map = initMap();
    //initialize other stuff such as vga, sd card, etc.
    alt_up_pixel_buffer_dma_dev *pixel_buffer = init_pixel_stuff("/dev/pixel_buffer_dma");
    Player* p1 = (Player*)malloc(sizeof(Player));

    p1->posX = 17;
    p1->posY = 17;
    p1->height = TILE_SIZE-2;
    p1->width = TILE_SIZE-2;
    p1->dropBomb = 0;
    p1->nBombs = 2;

    //Keep this here -Colin
    int connected = 0;
    while (connected == 0) {
        initSD(&connected);
    }

    //This is booting bmps -Colin
    Pixel_Map* booted_bmps = (Pixel_Map*) malloc(sizeof(Pixel_Map));
    booted_bmps = boot_bmps(booted_bmps);

    draw_screen_from_bmp(pixel_buffer, booted_bmps, 0, 0, 0);
    draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, 16, 16);
    draw_screen_from_bmp(pixel_buffer, booted_bmps, 2, 32, 32);
    draw_screen_from_bmp(pixel_buffer, booted_bmps, 3, 48, 48);
    draw_screen_from_bmp(pixel_buffer, booted_bmps, 4, 64, 64);
    draw_screen_from_bmp(pixel_buffer, booted_bmps, 5, 80, 80);

    //clock_t start, stop;
    // while (1) {
        // //start = clock();

     //    handleEvents(p1, IORD(switches, 0));

     //    move(p1, map);

     //    renderMap(map, pixel_buffer);
     //    renderPlayer (p1, pixel_buffer);
     //    refresh(pixel_buffer);



        // //handle inputs (adjust player velocity, drop a fruit)
        // //move the players (change x,y coordinates, check collisions)
        // //render everything onto the screen



        // //CAP FRAMERATE
        // /*stop = clock();
        // printf("%f\n", (((float)stop - (float)start) / CLOCKS_PER_SEC));
        // if ((((float)stop - (float)start) / CLOCKS_PER_SEC) > TICKS_PER_FRAME) {
        //  usleep((TICKS_PER_FRAME - (((float)stop - (float)start) / CLOCKS_PER_SEC)));
        // }*/

        // IOWR(leds, 0, IORD(switches, 0));
    // }
    destroyMap(map);
    return 0;
}
