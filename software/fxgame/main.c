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
#include "Menu.h"
#include "animations.h"
#include "sdcard.h"
#include "AI.h"
#include "Sound.h"
#include "sys/alt_timestamp.h"

#define switches (volatile char *) 0x0004430
#define leds (char *) 0x0004420

#define FPS 2.0
#define TICKS_PER_FRAME (1000.0 / FPS)
#define PS 256

int main() {
	menu = malloc(sizeof(Menu));
    mapTile** map = initMap();

    //initialize other stuff such as vga, sd card, etc.
    alt_up_pixel_buffer_dma_dev *pixel_buffer = init_pixel_stuff("/dev/pixel_buffer_dma");
    alt_up_char_buffer_dev *char_buffer = init_char_stuff("/dev/char_drawer");

    Score* score = (Score*)malloc(sizeof(Score));

    FruitCtrl* fruitCtrl = malloc(sizeof(FruitCtrl));
    initFruits(fruitCtrl,map,score);

    Player* p1 = (Player*)malloc(sizeof(Player));
    players = (Players*)malloc(sizeof(Players));

    p1->posX = 16;
    p1->posY = 16;
    p1->height = TILE_SIZE-2;
    p1->width = TILE_SIZE-2;
    p1->respawnTime = 0;
    p1->stunnedTime = 0;
    p1->dropBomb = 0;
    p1->id = 0;
    p1->leftKey = 'A';
    p1->rightKey = 'D';
    p1->upKey = 'W';
    p1->downKey = 'S';
    p1->fruitKey = 'Q';
    p1->score = &(score->scores[p1->id]);
    p1->fruitCtrl = fruitCtrl;
    p1->velX = 0;
    p1->velY = 0;
    p1->dir = left;
    p1->pwrUps = 0;
    p1->map = map;
    p1->bullCount = 0;

    Player* p2 = (Player*)malloc(sizeof(Player));

    p2->posX = 16+TILE_SIZE*17;
    p2->posY = 16+TILE_SIZE*12;
    p2->height = TILE_SIZE-2;
    p2->width = TILE_SIZE-2;
    p2->dropBomb = 0;
    p2->respawnTime = 0;
    p2->stunnedTime = 0;
    p2->id = 1;
    p2->leftKey = '4';
    p2->rightKey = '6';
    p2->upKey = '8';
    p2->downKey = '5';
    p2->fruitKey = '7';
    p2->score = &(score->scores[p2->id]);
    p2->fruitCtrl = fruitCtrl;
    p2->velX = 0;
    p2->velY = 0;
    p2->dir = left;
    p2->pwrUps = 0;
    p2->map = map;
    p2->bullCount = 0;
    
    /*AI* ai1 = (AI*)malloc(sizeof(AI));
    ai1->posX = 16+TILE_SIZE*17;
    ai1->posY = 16+TILE_SIZE*12;
    ai1->next = 0;
    ai1->end = 0;
    ai1->height = TILE_SIZE-2;
    ai1->width = TILE_SIZE-2;
    ai1->dropBomb = 0;
    ai1->respawnTime = 0;
    ai1->id = 1;
    ai1->lives = 10;
    ai1->velX = 0;
    ai1->velY = 0;
    ai1->move = 0;
    ai1->state = IDLE;*/

    players->list[p1->id] = p1;
    players->list[p2->id] = p2;

    printf("Initializing Setup \n");

    kbd_init(players);
    printf("Ready for key presses\n");

    //Keep this here -Colin
    int connected = 0;
    while (connected == 0) {
        initSD(&connected);
    }

    //This is booting bmps -Colin
    booted_bmps = (Pixel_Map*) malloc(sizeof(Pixel_Map));
    booted_bmps = boot_bmps(booted_bmps);

    printf("Booted images\n");

//    //Init sound
//    initSound(sb);
//
//    //Init fx sounds
//    initSoundFX(sb);
//
//    //Init bg sound
//    initSoundBG(sb);
//
//    //Init sound interrupt
//    initSoundFinal(sb);

    alt_up_char_buffer_clear(char_buffer);
    initMenu(menu);

    while (1) {
    	runMenu(menu, char_buffer);
    	renderMap(map, pixel_buffer);
    	refresh(pixel_buffer);
    	renderMap(map, pixel_buffer);
    	refresh(pixel_buffer);
    	//init timer//
    	printf("Initializing Timer \n");
    	initTimer(score, char_buffer);

    	switch(menu->timeMode) {
    	case(MIN_1):
    		score->timeLeft = 60;
    		break;
    	case(MIN_3):
    		score->timeLeft = 180;
    		break;
    	case(MIN_5):
    		score->timeLeft = 300;
    		break;
    	default:
    		score->timeLeft = 599;
    		break;
    	}
    	score->map = map;
    	printf("Done timer: \n");

    	chooseFruitForPlayer(p1, fruitCtrl, (fruitType)menu->p1Mode);
    	chooseFruitForPlayer(p2, fruitCtrl, (fruitType)menu->p2Mode);

    	alt_timestamp_start();
    	while (1) {
    		while (alt_timestamp() < 3333333);
    		alt_timestamp_start();
    		handleEvents(p1);
    		handleEvents(p2);
    		//handleAI(ai1, fruitCtrl, p1);
    		updateFruits(fruitCtrl);
    		updatePlayer(p1);
    		updatePlayer(p2);
    		//updateAI(ai1);
    		renderMap(map, pixel_buffer);
    		renderPlayer (p1, pixel_buffer);
    		renderPlayer (p2, pixel_buffer);
    		//renderAI (ai1, pixel_buffer);
    		int ret = renderScore (score, char_buffer);
    		refresh(pixel_buffer);
    		if (!ret)
    			break;
    	}
    	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 96, 96, 96+128, 96+48, 0x0000,0);
    	gameOver(score, char_buffer);
    }

    destroyMap(map);
    return 0;
}
