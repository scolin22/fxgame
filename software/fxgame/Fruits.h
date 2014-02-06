#ifndef FRUITS_H
#define FRUITS_H

#include "Map.h"
#include "Types.h"
#define FRUITS_PER_PLAYER 5
#define INIT_FRUITS 2
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
    int tossed;
    tile_t tileOn;
} Fruit;

typedef struct FruitCtrl {
    Fruit fruits[FRUITS_PER_PLAYER*NUM_PLAYERS];
    int startIndex[NUM_PLAYERS];
    int maxFruits[NUM_PLAYERS];
    int numFruits[NUM_PLAYERS];
    mapTile** map;
} FruitCtrl;

extern FruitCtrl* fruitCtrl;

void initFruits(FruitCtrl *fruitCtrl, mapTile** d);
void printFruits(FruitCtrl *fruitCtrl);
void updateFruits(FruitCtrl *fruitCtrl);
tile_t checkExplosion(FruitCtrl *fruitCtrl, int x, int y);
void explodeFruit(FruitCtrl *fruitCtrl, Fruit fruit);
void cleanExplosion(FruitCtrl *fruitCtrl, Fruit fruit);
void increaseFruitRadius(FruitCtrl *fruitCtrl, int owner);
char dropFruit(FruitCtrl *fruitCtrl, int owner, char toss, direction dir,int x, int y);
void moveFruit(Fruit* fruit);
char checkFruitCollision (Fruit* f);
char checkThrowCollision (Fruit* f);
Fruit* checkForFruitAtPosition(int x, int y);

#endif //FruitCtrl.h
