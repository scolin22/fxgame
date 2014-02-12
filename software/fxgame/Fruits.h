#ifndef FRUITS_H
#define FRUITS_H

#include "Score.h"
#include "Map.h"
#include "Types.h"
#define FRUITS_PER_PLAYER 10
#define INIT_FRUITS 1
#define FRUIT_TIMEOUT 30
#define EXPLOSION_TIMEOUT 15
#define DEFAULT_RADIUS 1

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
    fruitType types[NUM_PLAYERS];
    mapTile** map;
    Score* score;
    int counter;
    int counterMax;
} FruitCtrl;

void initFruits(FruitCtrl *fruitCtrl, mapTile** d, Score* score);
void chooseFruitForPlayer(FruitCtrl* fruitCtrl, fruitType type, int owner);
void printFruits(FruitCtrl *fruitCtrl);
void updateFruits(FruitCtrl *fruitCtrl);
tile_t checkExplosion(FruitCtrl *fruitCtrl, int x, int y);
void explodeFruit(FruitCtrl *fruitCtrl, Fruit fruit);
void cleanExplosion(FruitCtrl *fruitCtrl, Fruit fruit);
void increaseFruitCount(FruitCtrl *fruitCtrl, int owner);
void increaseFruitRadius(FruitCtrl *fruitCtrl, int owner);
char dropFruit(FruitCtrl *fruitCtrl, int owner, char toss, direction dir,int x, int y);
void moveFruit(mapTile** map, Fruit* fruit, FruitCtrl* fruitCtrl);
char checkFruitCollision (mapTile** map, Fruit* f);
char checkThrowCollision (mapTile** map, Fruit* f);
Fruit* checkForFruitAtPosition(FruitCtrl *fruitCtrl, int x, int y);
int getFruitRadius(FruitCtrl* fruitCtrl, int owner);
fruitType getFruitType(FruitCtrl* fruitCtrl, int owner);

#endif //FruitCtrl.h
