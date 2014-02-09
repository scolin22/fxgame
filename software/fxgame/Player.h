#ifndef PLAYER_H
#define PLAYER_H

#include "Types.h"
#include "animations.h"
#include "Fruits.h"
#include "keyboard.h"
#include "Map.h"
#include "PowerUps.h"

#define RESPAWN_TIME 30
static const int VELOCITY = 5;

typedef struct Player {
    int posX; // x coordinate of the player (NOT the tile coordinate)
    int posY; // y coordinate of the player (NOT the tile coordinate)
    int respawnTime;
    int score;
    char width; // width of player (required for collision detection).. this should be the width of the player sprite
    char height; // height of player (required for collision detection
    char velX; // velocity of the player
    char velY;
    char dropBomb;
    int id;
    char leftKey;
    char rightKey;
    char upKey;
    char downKey;
    char fruitKey;
    direction dir;
    FruitCtrl *fruitCtrl;
    powerUps pwrUps;
    mapTile** map;
} Player;

typedef struct Players {
	Player* list[NUM_PLAYERS];
} Players;

extern Players *players;

void handleEvents (Player* p);
void move (Player* p);
void movePress (Player* p, char ascii);
void render (Player* p);
void renderPlayer (Player* p, alt_up_pixel_buffer_dma_dev *pixel_buffer);
char checkCollision (Player* p, direction dir);
char checkPowerUps(Player *p, powerUps pwrUp);
void setPowerUps(Player *p, powerUps pwrUp);

#endif //PLAYER_H
