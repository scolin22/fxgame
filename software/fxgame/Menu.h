/*
 * Menu.h
 *
 *  Created on: 2014-02-12
 *      Author: Byron
 */

#include <stdio.h>
#include <unistd.h>
#include <system.h>
#include "animations.h"
#include "Fruits.h"
#include "Score.h"

#ifndef MENU_H_
#define MENU_H_

#define CURSOR_X 10
#define CURSOR_HEIGHT 10
#define CURSOR_WIDTH 10
#define MENU_TITLE_Y 15

typedef enum {
	MAIN_MENU,
	INSTRUCTIONS,
	OPTIONS,
	GAME,
	GAMEOVER
}MENU_MODE;

typedef enum {
	PLAYER_OFF,
	BANANA,
	CHERRY,
	ORANGE,
	WATERMELON
}PLAYER_MODE;

typedef enum {
	AI_OFF,
	AI_1,
	AI_2
}AI_MODE;

typedef enum {
	MIN_1,
	MIN_3,
	MIN_5,
	MIN_10
}TIME_LIMIT;

typedef struct Menu {
	volatile int optionSelected;
	int optionsMax;
	char hasChanged;
	volatile MENU_MODE mode;
	PLAYER_MODE p1Mode;
	PLAYER_MODE p2Mode;
	AI_MODE aiMode;
	TIME_LIMIT timeMode;
}Menu;

void runMenu(Menu* menu, alt_up_char_buffer_dev* char_buffer);
int getCursorPosition(Menu*);
void initMenu(Menu* menu);
void handleMenu(char ascii);

extern Menu* menu;

#endif /* MENU_H_ */
