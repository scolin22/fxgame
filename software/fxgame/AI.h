#ifndef AI_H
#define AI_H

#include "animations.h"
#include "Fruits.h"
#include "keyboard.h"
#include "Player.h"
#include "Map.h"

#define FIND_CRATE_ARRAY_SIZE 100
#define LONGEST_PATH 20

typedef enum {
    IDLE,
    RUN_FRUIT, // typical avoid fruit state
    FIND_CRATE, // move towards spot to destroy crate
    DESTORY_CRATE, // drop fruit to destory crate
    HIDE_FRUIT_CRATE, // hide from fruit placed to destroy crate
    HUNT_PLAYER,
    DESTORY_PLAYER,
    HIDE_FRUIT_PLAYER
} state_t;

typedef struct AI {
    int posX; // x coordinate of the player (NOT the tile coordinate)
    int posY; // y coordinate of the player (NOT the tile coordinate)
    int respawnTime;
    int lives;
    char width; // width of player (required for collision detection).. this should be the width of the player sprite
    char height; // height of player (required for collision detection
    char velX; // velocity of the player
    char velY;
    char id;

    char dropBomb;
    int destY[LONGEST_PATH];
    int destX[LONGEST_PATH];
    int next;
    int end;
    char move;
    state_t state;
    int bombTicks;
    char map[NTILEY][NTILEX];
} AI;

typedef struct Path {
    char x;
    char y;
    direction d;
    char prevIndex;
    char length;
} Path;

void decide (AI* a, FruitCtrl* fruitCtrl, Player* p);

void preExplodeMap (AI* a, FruitCtrl* f);
void preExplodeFruit (AI* a, FruitCtrl* f, Fruit fruit);
char preCheckExplosion (AI* a, int x, int y);

char checkSafe (AI* a, int x, int y);
char checkSteppable (AI* a, FruitCtrl* f, int x, int y);
char targetClosestSafeSpot (AI* a, FruitCtrl* f);

char targetMostCrates (AI* a, FruitCtrl* f, char d);
char preExplodeCrateCount (AI* a, int x, int y);

char targetClosestCrate (AI* a); //UPDATE

char targetPlayer (AI* a, FruitCtrl* f, Player* p);
char preExplodePlayerHit (AI* a, int x, int y, Player* p);


void handleAI (AI* a, FruitCtrl* fruitCtrl, Player* p);
void moveAI (AI* a, FruitCtrl* fruitCtrl);
void renderAI (AI* a, alt_up_pixel_buffer_dma_dev *pixel_buffer);
void updateAI(AI* a);

#endif //AI_H
