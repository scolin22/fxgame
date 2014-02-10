#include <stdlib.h>
#include <stdio.h>

#include "Fruits.h"

int counter = 0;

void initFruits(FruitCtrl *fruitCtrl, mapTile** d, Score* score) {
    int i = 0;
    fruitCtrl->score = score;
    fruitCtrl->map = d;
    while (i < NUM_PLAYERS) {
        fruitCtrl->startIndex[i] = i*FRUITS_PER_PLAYER;
        fruitCtrl->numFruits[i] = 0;
        fruitCtrl->maxFruits[i] = INIT_FRUITS;
        int j;
        for (j = i*FRUITS_PER_PLAYER; j < (i+1)*FRUITS_PER_PLAYER; j++) {
            fruitCtrl->fruits[j].owner = i;
            fruitCtrl->fruits[j].status = hidden;
            fruitCtrl->fruits[j].radius = DEFAULT_RADIUS;
            fruitCtrl->fruits[j].timeLeft = 0;
            fruitCtrl->fruits[j].tossed = 0;
            fruitCtrl->fruits[j].velX = 0;
            fruitCtrl->fruits[j].velY = 0;
            fruitCtrl->fruits[j].posX = 0;
            fruitCtrl->fruits[j].posY = 0;
            fruitCtrl->fruits[j].tileOn = GRASS;
        }
        i++;
    }
}

void increaseFruitRadius(FruitCtrl *fruitCtrl, int owner) {
    int i;
    for (i = owner*FRUITS_PER_PLAYER; i < (owner+1)*FRUITS_PER_PLAYER; i++) {
        fruitCtrl->fruits[i].radius++;
    }
}

void printFruits(FruitCtrl *fruitCtrl) {
    int i;
    for (i = 0; i < NUM_PLAYERS*FRUITS_PER_PLAYER; i++) {
        printf("%d,%d,%d,%d\n", fruitCtrl->fruits[i].owner, fruitCtrl->fruits[i].status, fruitCtrl->fruits[i].radius, fruitCtrl->fruits[i].timeLeft);
    }
    printf("\n\n");
}

void updateFruits(FruitCtrl *fruitCtrl) {
    int i;
    for (i = 0; i < NUM_PLAYERS*FRUITS_PER_PLAYER; i++) {
        if (fruitCtrl->fruits[i].timeLeft > 0) {
            fruitCtrl->fruits[i].timeLeft--;
            if (fruitCtrl->fruits[i].timeLeft <= 0) {
                if (fruitCtrl->fruits[i].status == active) {
                    fruitCtrl->fruits[i].status = exploding;
                    fruitCtrl->fruits[i].timeLeft = EXPLOSION_TIMEOUT;
                    explodeFruit(fruitCtrl, fruitCtrl->fruits[i]);
                } else if (fruitCtrl->fruits[i].status == exploding) {
                    fruitCtrl->fruits[i].status = hidden;
                    if (fruitCtrl->numFruits[fruitCtrl->fruits[i].owner] >= 1)
                        fruitCtrl->numFruits[fruitCtrl->fruits[i].owner] -= 1;
                    cleanExplosion(fruitCtrl, fruitCtrl->fruits[i]);
//                  for (i = 0; i < NUM_PLAYERS*FRUITS_PER_PLAYER; i++)
//                     if (fruitCtrl->fruits[i].timeLeft > 0 && fruitCtrl->fruits[i].status == exploding)
//                         explodeFruit(fruitCtrl, fruitCtrl->fruits[i]);
                }
            } else if (fruitCtrl->fruits[i].velX != 0 || fruitCtrl->fruits[i].velY != 0) {
                moveFruit(fruitCtrl->map, &(fruitCtrl->fruits[i]));
            }
        }
    }
}


tile_t checkExplosion(FruitCtrl *fruitCtrl, int x, int y) {
    tile_t tile = checkType(fruitCtrl->map, x, y);
    if (tile == BLOCK || tile == END) {
        return BLOCK;
    } else if (tile == CRATE) {
        return CRATE;
    } else if (tile == FRUIT) {
        int i;
        for (i = 0; i < NUM_PLAYERS*FRUITS_PER_PLAYER; i++) {
            if(fruitCtrl->fruits[i].posX == x && fruitCtrl->fruits[i].posY == y) {
                if (fruitCtrl->fruits[i].status == active) {
                    fruitCtrl->fruits[i].status = exploding;
                    fruitCtrl->fruits[i].timeLeft = EXPLOSION_TIMEOUT;
                    explodeFruit(fruitCtrl, fruitCtrl->fruits[i]);
                    return FRUIT;
                }
            }
        }
    }
    return GRASS;
}


char explodeTile(FruitCtrl* fruitCtrl, int x, int y, tile_t tile, int owner) {
    if (tile == BLOCK) {
        return 0;
    } else if (tile == CRATE) {
    	printf("blowing up crate with %d who has score %d\n", owner, fruitCtrl->score->scores[owner]);
        changeTile(fruitCtrl->map, x, y, 6 + counter++);
        fruitCtrl->score->scores[owner] += 10;
        if (counter == 9) counter = 0;
        return 0;
    } else if (tile == FRUIT) {
        changeTileWithOwner(fruitCtrl->map, x, y, EXPLOSION, owner);
        return 0;
    } else {
    	changeTileWithOwner(fruitCtrl->map, x, y, EXPLOSION, owner);
        return 1;
    }
}

void explodeFruit(FruitCtrl *fruitCtrl, Fruit fruit) {

    if(fruit.tileOn == BLOCK)
    	changeTile(fruitCtrl->map, fruit.posX, fruit.posY, BLOCK);
    else if (fruit.tileOn = FRUIT && (checkForFruitAtPosition(fruitCtrl, fruit.posX, fruit.posY)->tileOn == BLOCK)) {
    	changeTile(fruitCtrl->map, fruit.posX, fruit.posY, BLOCK);
    }
    else
    	changeTileWithOwner(fruitCtrl->map, fruit.posX, fruit.posY, EXPLOSION, fruit.owner);


    tile_t currentTile;
    int x, y;
    //Check left
    int i;
    y = fruit.posY;
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        x = fruit.posX - i;
        currentTile = checkExplosion(fruitCtrl, x, y);
        if (!explodeTile(fruitCtrl, x, y, currentTile, fruit.owner))
            break;
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        x = fruit.posX + i;
        currentTile = checkExplosion(fruitCtrl, x, y);
        if (!explodeTile(fruitCtrl, x, y, currentTile, fruit.owner))
            break;
    }
    x = fruit.posX;
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        y = fruit.posY - i;
        currentTile = checkExplosion(fruitCtrl, x, y);
        if (!explodeTile(fruitCtrl, x, y, currentTile, fruit.owner))
            break;
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        y = fruit.posY + i;
        currentTile = checkExplosion(fruitCtrl, x, y);
        if (!explodeTile(fruitCtrl, x, y, currentTile, fruit.owner))
            break;
    }
}

void cleanExplosion(FruitCtrl *fruitCtrl, Fruit fruit) {
    checkExplosion(fruitCtrl, fruit.posX, fruit.posY);
    if (checkType(fruitCtrl->map, fruit.posX, fruit.posY) == EXPLOSION)
        changeTile(fruitCtrl->map, fruit.posX, fruit.posY, GRASS);

    //Check left
    int i, x, y;
    y = fruit.posY;
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        x = fruit.posX - i;
        if (checkType(fruitCtrl->map, x, y) == EXPLOSION)
            changeTile(fruitCtrl->map, x, y, GRASS);
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        x = fruit.posX + i;
        if (checkType(fruitCtrl->map, x, y) == EXPLOSION)
            changeTile(fruitCtrl->map, x, y, GRASS);
    }
    x = fruit.posX;
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        y = fruit.posY - i;
        if (checkType(fruitCtrl->map, x, y) == EXPLOSION)
            changeTile(fruitCtrl->map, x, y, GRASS);
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        y = fruit.posY + i;
        if (checkType(fruitCtrl->map, x, y) == EXPLOSION)
            changeTile(fruitCtrl->map, x, y, GRASS);
    }
}

char dropFruit(FruitCtrl *fruitCtrl, int owner, char toss, direction dir, int x, int y){
    if (checkType (fruitCtrl->map, x, y) == FRUIT && toss) {
        Fruit* fruit = checkForFruitAtPosition(fruitCtrl, x,y);
        if(dir == right)
            fruit->velX = 1;
        else if(dir == left)
            fruit->velX = -1;
        else if(dir == up)
            fruit->velY = -1;
        else if(dir == down)
            fruit->velY = 1;
        fruit->tossed = 4;
        return 0;
    }
    if (checkType (fruitCtrl->map, x, y) == FRUIT || fruitCtrl->numFruits[owner] >= fruitCtrl->maxFruits[owner])
        return 0;
    int i;
    for (i = fruitCtrl->startIndex[owner]; i < (owner < NUM_PLAYERS - 1 ? fruitCtrl->startIndex[owner + 1] : NUM_PLAYERS*FRUITS_PER_PLAYER); i++) {
        if (fruitCtrl->fruits[i].status == hidden) {
            fruitCtrl->fruits[i].status = active;
            fruitCtrl->fruits[i].owner = owner;
            fruitCtrl->fruits[i].posX = x;
            fruitCtrl->fruits[i].posY = y;
            fruitCtrl->fruits[i].timeLeft = FRUIT_TIMEOUT;
            fruitCtrl->fruits[i].tileOn = GRASS;
            fruitCtrl->numFruits[owner] += 1;
            changeTile(fruitCtrl->map, x, y, FRUIT);
            break;
        }
    }
    return 1;
}

void moveFruit(mapTile** map, Fruit* fruit){
    changeTile(map, fruit->posX, fruit->posY, GRASS);
    set_db(map, fruit->posX, fruit->posY);

    int initX = fruit->posX;
    int initY = fruit->posY;

    if (fruit->status != active) {
        fruit->velX = 0;
        fruit->velY = 0;
        fruit->tossed = 0;
        return;
    }
    if (fruit-> tossed == 1) {
        fruit->velX = 0;
        fruit->velY = 0;
        fruit->tossed = 0;
        changeTile(map, fruit->posX, fruit->posY, FRUIT);
        return;
    }
    if (fruit->velX > 0)
        fruit->posX += TILE_SIZE;
    else if (fruit->velX < 0)
        fruit->posX -= TILE_SIZE;
    else if (fruit->velY > 0)
        fruit->posY += TILE_SIZE;
    else if (fruit->velY < 0)
        fruit->posY -= TILE_SIZE;

    if (fruit->tossed > 1 ) {
        fruit->tossed--;
        if (checkThrowCollision(map, fruit)) {
            fruit->posX = initX;
            fruit->posY = initY;
            fruit->velX = 0;
            fruit->velY = 0;
            fruit->tossed = 0;
        } else {
            changeTile(map, initX, initY, fruit->tileOn);
            fruit->tileOn = checkType(map, fruit->posX, fruit->posY);
        }
    } else if (checkFruitCollision(map, fruit)) {
        fruit->posX = initX;
        fruit->posY = initY;
        fruit->velX = 0;
        fruit->velY = 0;
        fruit->tossed = 0;
        printf("didn't kick\n");
    } else {
    	printf("did kick\n");
    }
    changeTile(map, fruit->posX, fruit->posY, FRUIT);
}

char checkFruitCollision (mapTile** map, Fruit* f)
{
    tile_t tile = checkType(map, f->posX, f->posY);
    if (f->posX < 0 || f->posY < 0 || (f->posX+TILE_SIZE) >= SCREEN_WIDTH || (f->posY+TILE_SIZE) >= SCREEN_HEIGHT)
        return 1;
    else if ((tile != GRASS && tile != EXPLOSION) || map[(int)y_to_ty(f->posY)][x_to_tx(f->posX)].playerOn == 1) {
    	printf("the tile is %d\n", tile);
        return 1;
    }
    return 0;
}
char checkThrowCollision (mapTile** map, Fruit* f)
{
    tile_t tile = checkType(map, f->posX, f->posY);
    if (f->posX < 0 || f->posY < 0 || (f->posX+TILE_SIZE) >= SCREEN_WIDTH || (f->posY+TILE_SIZE) >= SCREEN_HEIGHT)
        return 1;
    else if (tile == END)
        return 1;
    return 0;
}

Fruit* checkForFruitAtPosition(FruitCtrl *fruitCtrl, int x, int y) {
    int i;
    for (i = 0; i < NUM_PLAYERS*FRUITS_PER_PLAYER; i++)
        if (fruitCtrl->fruits[i].posX == x && fruitCtrl->fruits[i].posY == y)
            return &(fruitCtrl->fruits[i]);
        return NULL;
    }
