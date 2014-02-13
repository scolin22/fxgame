#include <stdio.h>
#include <unistd.h>
#include <system.h>
#include <io.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>

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
    mapTile** map = allocateMap();

    //initialize other stuff such as vga, sd card, etc.
    alt_up_pixel_buffer_dma_dev *pixel_buffer = init_pixel_stuff("/dev/pixel_buffer_dma");
    alt_up_char_buffer_dev *char_buffer = init_char_stuff("/dev/char_drawer");

    Score* score = (Score*)malloc(sizeof(Score));

    FruitCtrl* fruitCtrl = malloc(sizeof(FruitCtrl));
    initFruits(fruitCtrl,map,score);

    players = (Players*)malloc(sizeof(Players));

    Player* p1 = (Player*)malloc(sizeof(Player));

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
    
    AI* ai1 = (AI*)malloc(sizeof(AI));
    ai1->posX = 16;
    ai1->posY = 16+TILE_SIZE*12;
    ai1->next = 0;
    ai1->end = 0;
    ai1->height = TILE_SIZE-2;
    ai1->width = TILE_SIZE-2;
    ai1->dropBomb = 0;
    ai1->respawnTime = 0;
    ai1->stunnedTime = 0;
    ai1->id = 2;
    ai1->lives = 10;
    ai1->velX = 0;
    ai1->velY = 0;
    ai1->move = 0;
    ai1->state = IDLE;
    ai1->dir = left;
    ai1->pwrUps = 0;
    ai1->score = &(score->scores[ai1->id]);
    ai1->fruitCtrl = fruitCtrl;

    AI* ai2 = (AI*)malloc(sizeof(AI));
    ai2->posX = 16+TILE_SIZE*17;
    ai2->posY = 16;
    ai2->next = 0;
    ai2->end = 0;
    ai2->height = TILE_SIZE-2;
    ai2->width = TILE_SIZE-2;
    ai2->dropBomb = 0;
    ai2->respawnTime = 0;
    ai2->stunnedTime = 0;
    ai2->id = 3;
    ai2->lives = 10;
    ai2->velX = 0;
    ai2->velY = 0;
    ai2->move = 0;
    ai2->state = IDLE;
    ai2->dir = left;
    ai2->pwrUps = 0;
    ai2->score = &(score->scores[ai2->id]);
    ai2->fruitCtrl = fruitCtrl;

    players->list[p1->id] = p1;
    players->list[p2->id] = p2;
    scores[0] = ai1->score;
    scores[1] = ai2->score;

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

    //Init sound
    initSound(sb);

    //Init fx sounds
//    initSoundFX(sb);

    //Init bg sound
    initSoundBG(sb);

    //Init sound interrupt
    initSoundFinal(sb);

    initMenu(menu);

    while (1) {
    	alt_timestamp_start();
    	printf("game done\n");
        alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
        alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);
    	alt_up_char_buffer_clear(char_buffer);
    	runMenu(menu, char_buffer, pixel_buffer);
    	srand(alt_timestamp());
    	initMap(map);
    	initFruits(fruitCtrl,map,score);
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
    	chooseFruitForAI(ai1, fruitCtrl, cherry);
    	chooseFruitForAI(ai2, fruitCtrl, cherry);

    	struct timeval tm;

    	alt_timestamp_start();
    	while (1) {
    		while (alt_timestamp() < 3333333);
    		alt_timestamp_start();

    		if (menu->p1Mode != PLAYER_OFF)
    			handleEvents(p1);
    		if (menu->p2Mode != PLAYER_OFF)
    			handleEvents(p2);
    		if (menu->aiMode != AI_OFF)
    			handleAI(ai1, (menu->p1Mode != PLAYER_OFF) ? p1 : 0, (menu->p2Mode != PLAYER_OFF) ? p2 : 0, 0);
    		if (menu->aiMode == AI_2)
    			handleAI(ai2, (menu->p1Mode != PLAYER_OFF) ? p1 : 0, (menu->p2Mode != PLAYER_OFF) ? p2 : 0, 0);
    		//handleAI(ai1, fruitCtrl, p1);
    		updateFruits(fruitCtrl);
    		if (menu->p1Mode != PLAYER_OFF)
    			updatePlayer(p1);
    		if (menu->p2Mode != PLAYER_OFF)
    			updatePlayer(p2);
    		if (menu->aiMode != AI_OFF)
    			updateAI(ai1);
    		if (menu->aiMode == AI_2)
    			updateAI(ai2);
    		renderMap(map, pixel_buffer);
    		if (menu->p1Mode != PLAYER_OFF)
    			renderPlayer (p1, pixel_buffer);
    		if (menu->p2Mode != PLAYER_OFF)
    			renderPlayer (p2, pixel_buffer);
    		if (menu->aiMode != AI_OFF)
    			renderAI (ai1, pixel_buffer);
    		if (menu->aiMode == AI_2)
    			renderAI (ai2, pixel_buffer);
    		int ret = renderScore (score, char_buffer);
    		refresh(pixel_buffer);
    		if (!ret)
    			break;
    	}
    	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 96, 96, 96+128, 96+48, 0x0000,0);
    	gameOver(score, char_buffer);
    	printf("game done2\n");
    	menu->mode = GAMEOVER;
    	while (menu->mode == GAMEOVER);
    	menu->hasChanged = 2;
    	menu->optionsMax = 3;
    	resetPlayer1(p1);
    	resetPlayer2(p2);
    	resetAI1(ai1);
    	resetAI2(ai2);
    }
    return 0;
}
