#ifndef AI_H
#define AI_H

#include "animations.h"
#include "Fruits.h"
#include "keyboard.h"
#include "Player.h"
#include "Map.h"

#define PATH_ARRAY_SIZE 100
#define LONGEST_PATH 20

typedef enum {
    IDLE,
    AVOID_FRUIT, // typical avoid fruit state
    FIND_CRATE, // move towards spot to destroy crate
    HUNT_PLAYER, // move towards spot where I can attack player
    DESTORY, // drop bomb on current spot
    HIDE // run and hide
} state_t;

typedef struct AI {
    int posX; // x coordinate of the player (NOT the tile coordinate)
    int posY; // y coordinate of the player (NOT the tile coordinate)
    int respawnTime;
    int stunnedTime;
    int lives;
    char width; // width of player (required for collision detection).. this should be the width of the player sprite
    char height; // height of player (required for collision detection
    char velX; // velocity of the player
    char velY;
    char id;
    direction dir;

    int* score;
    powerUps pwrUps;

    FruitCtrl* fruitCtrl;
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
    char prevIndex;
    char length;
} Path;

void decide (AI* a, Player* p);

void preExplodeMap (AI* a);
void preExplodeFruit (AI* a, Fruit fruit);
char preCheckExplosion (AI* a, int x, int y, int owner);

char checkSafe (AI* a, int x, int y);
char checkSteppable (AI* a, int x, int y);
char targetClosestSafeSpot (AI* a);

char targetMostCrates (AI* a, char d);
char preExplodeCrateCount (AI* a, int x, int y);

char targetPlayer (AI* a, Player* p);
char preExplodePlayerHit (AI* a, int x, int y, Player* p);


void handleAI (AI* a, Player* p, char switches);
void moveAI (AI* a);
void renderAI (AI* a, alt_up_pixel_buffer_dma_dev *pixel_buffer);
void updateAI(AI* a);

#endif //AI_H
