#ifndef FRUITS_H
#define FRUITS_H

#include "Map.h"

#define FRUITS_PER_PLAYER 10
#define INIT_FRUITS 10
#define NUM_PLAYERS 2
#define FRUIT_TIMEOUT 30
#define EXPLOSION_TIMEOUT 15
#define DEFAULT_RADIUS 3

typedef enum fruitStatus {
    hidden,
    active,
    exploding,
} fruitStatus;

typedef struct Fruit {
    int timeLeft;
    fruitStatus status;
    int radius;
    int owner;
    int posX;
    int posY;
    int velX;
    int velY;
} Fruit;

typedef struct FruitCtrl {
    Fruit fruits[FRUITS_PER_PLAYER*NUM_PLAYERS];
    int startIndex[NUM_PLAYERS];
    int maxFruits[NUM_PLAYERS];
    int numFruits[NUM_PLAYERS];
    mapTile** map;
} FruitCtrl;

void initFruits(FruitCtrl *fruitCtrl, mapTile** d);
void printFruits(FruitCtrl *fruitCtrl);
void updateFruits(FruitCtrl *fruitCtrl);
tile_t checkExplosion(FruitCtrl *fruitCtrl, int x, int y);
char explodeTile(mapTile** map, int x, int y, tile_t tile);
void explodeFruit(FruitCtrl *fruitCtrl, Fruit fruit);
void cleanExplosion(FruitCtrl *fruitCtrl, Fruit fruit);
char dropFruit(FruitCtrl *fruitCtrl, int owner, int x, int y);
char checkFruitCollision (FruitCtrl* fruitCtrl, Fruit* f);
Fruit* checkForFruitAtPosition(FruitCtrl* fruitCtrl, int x, int y);
void moveFruit(FruitCtrl* fruitCtrl, Fruit* fruit);

#endif //FruitCtrl.h
