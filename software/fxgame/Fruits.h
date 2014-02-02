#ifndef FRUITS_H
#define FRUITS_H

#include "Map.h"

#define FRUITS_PER_PLAYER 2
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
    int x_pos;
    int y_pos;
} Fruit;

typedef struct FruitCtrl {
    Fruit fruits[FRUITS_PER_PLAYER*NUM_PLAYERS];
    int startIndex[NUM_PLAYERS];
    int maxFruits[NUM_PLAYERS];
    int numFruits[NUM_PLAYERS];
    char** map;
} FruitCtrl;

void initFruits(FruitCtrl *fruitCtrl, char** d);
void printFruits(FruitCtrl *fruitCtrl);
void updateFruits(FruitCtrl *fruitCtrl);
char checkExplosion(FruitCtrl *fruitCtrl, int x, int y);
void explodeFruit(FruitCtrl *fruitCtrl, Fruit fruit);
void cleanExplosion(FruitCtrl *fruitCtrl, Fruit fruit);
char dropFruit(FruitCtrl *fruitCtrl, int owner, int x, int y);

#endif //FruitCtrl.h
