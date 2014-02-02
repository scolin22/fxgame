#ifndef PLAYER_H
#define PLAYER_H

#include "animations.h"

static const int VELOCITY = 5;

typedef struct Player {
	int posX; // x coordinate of the player (NOT the tile coordinate)
	int posY; // y coordinate of the player (NOT the tile coordinate)
	char width; // width of player (required for collision detection).. this should be the width of the player sprite
	char height; // height of player (required for collision detection
	char nBombs; // number of bombs player can place
	char velX; // velocity of the player
	char velY;
    char id;
} Player;

void handleEvents (Player* p, char switches);
void move (Player* p, char** map);
void render (Player* p);

char checkCollision (Player* p, char** map);

#endif //PLAYER_H
