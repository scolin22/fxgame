#ifndef PLAYER_H
#define PLAYER_H

#include "animations.h"
#include "Fruits.h"
#include "keyboard.h"
#include "Map.h"
#include "PowerUps.h"
#include "Types.h"

#define RESPAWN_TIME 30
static const int VELOCITY = 5;

typedef struct Player {
    int posX; // x coordinate of the player (NOT the tile coordinate)
    int posY; // y coordinate of the player (NOT the tile coordinate)
    int respawnTime;
    int lives;
    char width; // width of player (required for collision detection).. this should be the width of the player sprite
    char height; // height of player (required for collision detection
    char velX; // velocity of the player
    char velY;
    char dropBomb;
    char id;
    char leftKey;
    char rightKey;
    char upKey;
    char downKey;
    char fruitKey;
    direction dir;
    FruitCtrl *fruitCtrl;
    powerUps pwrUps;
} Player;

extern Player* p1;
extern Player* p2;

void handleEvents (Player* p);
void move (Player* p);
void movePress (Player* p, char ascii);
void render (Player* p);
void renderPlayer (Player* p, alt_up_pixel_buffer_dma_dev *pixel_buffer);
char checkCollision (Player* p, mapTile** map, direction dir);
char checkPowerUps(Player *p, powerUps pwrUp);
void setPowerUps(Player *p, powerUps pwrUp);

#endif //PLAYER_H
