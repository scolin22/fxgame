#include <stdlib.h>
#include <stdio.h>

#include "Fruits.h"

FruitCtrl* fruitCtrl;

void initFruits(FruitCtrl *fruitCtrl, mapTile** d) {
    int i = 0;
    fruitCtrl->map = d;
    while (i < NUM_PLAYERS) {
        fruitCtrl->startIndex[i] = i*FRUITS_PER_PLAYER;
        fruitCtrl->numFruits[i] = 0;
        fruitCtrl->maxFruits[i] = 2;
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
                moveFruit(&(fruitCtrl->fruits[i]));
            }
        }
    }
}


char checkExplosion(FruitCtrl *fruitCtrl, int x, int y) {
    if (checkType(fruitCtrl->map, x, y) == BLOCK || checkType(fruitCtrl->map, x, y) == END) {
        return 1;
    } else if (checkType(fruitCtrl->map, x, y) == CRATE) {
        return 2;
    } else if (checkType(fruitCtrl->map, x, y) == FRUIT) {
        int i;
        for (i = 0; i < NUM_PLAYERS*FRUITS_PER_PLAYER; i++) {
            if(fruitCtrl->fruits[i].posX == x && fruitCtrl->fruits[i].posY == y) {
                if (fruitCtrl->fruits[i].status == active) {
                    fruitCtrl->fruits[i].status = exploding;
                    fruitCtrl->fruits[i].timeLeft = EXPLOSION_TIMEOUT;
                    explodeFruit(fruitCtrl, fruitCtrl->fruits[i]);
                    return 2;
                }
            }
        }
    } else {
        return 0;
    }
}
void explodeFruit(FruitCtrl *fruitCtrl, Fruit fruit) {

    if(fruit.tileOn != BLOCK)
        changeTile(fruitCtrl->map, fruit.posX, fruit.posY, EXPLOSION);
    else
        changeTile(fruitCtrl->map, fruit.posX, fruit.posY, BLOCK);

    //Check left
    int i;
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.posX - i, fruit.posY) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.posX - i, fruit.posY)== 2) {
            changeTile(fruitCtrl->map, fruit.posX - i, fruit.posY, EXPLOSION);
            break;
        }
        else
            changeTile(fruitCtrl->map, fruit.posX - i, fruit.posY, EXPLOSION);
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.posX + i, fruit.posY) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.posX + i, fruit.posY)== 2) {
            changeTile(fruitCtrl->map, fruit.posX + i, fruit.posY, EXPLOSION);
            break;
        }
        else
            changeTile(fruitCtrl->map, fruit.posX + i, fruit.posY, EXPLOSION);
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.posX, fruit.posY - i) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.posX, fruit.posY - i)== 2) {
            changeTile(fruitCtrl->map, fruit.posX, fruit.posY - i, EXPLOSION);
            break;
        }
        else
            changeTile(fruitCtrl->map, fruit.posX, fruit.posY - i, EXPLOSION);
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.posX, fruit.posY + i) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.posX, fruit.posY + i)== 2) {
            changeTile(fruitCtrl->map, fruit.posX, fruit.posY + i, EXPLOSION);
            break;
        }
        else
            changeTile(fruitCtrl->map, fruit.posX, fruit.posY + i, EXPLOSION);
    }
}

void cleanExplosion(FruitCtrl *fruitCtrl, Fruit fruit) {
    checkExplosion(fruitCtrl, fruit.posX, fruit.posY);
    if (checkType(fruitCtrl->map, fruit.posX, fruit.posY) == EXPLOSION)
        changeTile(fruitCtrl->map, fruit.posX, fruit.posY, GRASS);

    //Check left
    int i;
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.posX - i, fruit.posY) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.posX - i, fruit.posY)== 2) {
            if (checkType(fruitCtrl->map, fruit.posX - i, fruit.posY) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.posX - i, fruit.posY, GRASS);
            break;
        }
        else {
            if (checkType(fruitCtrl->map, fruit.posX - i, fruit.posY) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.posX - i, fruit.posY, GRASS);
        }
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.posX + i, fruit.posY) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.posX + i, fruit.posY)== 2) {
            if (checkType(fruitCtrl->map, fruit.posX + i, fruit.posY) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.posX + i, fruit.posY, GRASS);
            break;
        }
        else {
            if (checkType(fruitCtrl->map, fruit.posX + i, fruit.posY) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.posX + i, fruit.posY, GRASS);
        }
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.posX, fruit.posY - i) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.posX, fruit.posY - i)== 2) {
            if (checkType(fruitCtrl->map, fruit.posX, fruit.posY - i) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.posX, fruit.posY - i, GRASS);
            break;
        }
        else {
            if (checkType(fruitCtrl->map, fruit.posX, fruit.posY - i) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.posX, fruit.posY - i, GRASS);
        }
    }

    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.posX, fruit.posY + i) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.posX, fruit.posY + i)== 2) {
            if (checkType(fruitCtrl->map, fruit.posX, fruit.posY + i) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.posX, fruit.posY + i, GRASS);
            break;
        }
        else {
            if (checkType(fruitCtrl->map, fruit.posX, fruit.posY + i) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.posX, fruit.posY + i, GRASS);
        }
    }

}

char dropFruit(FruitCtrl *fruitCtrl, int owner, char toss, direction dir, int x, int y){
    if (checkType (fruitCtrl->map, x, y) == FRUIT && toss) {
        Fruit* fruit = checkForFruitAtPosition(x,y);
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

void moveFruit(Fruit* fruit){
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
        if (checkThrowCollision(fruit)) {
            fruit->posX = initX;
            fruit->posY = initY;
            fruit->velX = 0;
            fruit->velY = 0;
            fruit->tossed = 0;
        } else {
            changeTile(map, initX, initY, fruit->tileOn);
            fruit->tileOn = checkType(map, fruit->posX, fruit->posY);
        }
    } else if (checkFruitCollision(fruit)) {
        fruit->posX = initX;
        fruit->posY = initY;
        fruit->velX = 0;
        fruit->velY = 0;
        fruit->tossed = 0;
    }
    changeTile(map, fruit->posX, fruit->posY, FRUIT);
}

char checkFruitCollision (Fruit* f)
{
    tile_t mapTile = checkType(map, f->posX, f->posY);
    if (f->posX < 0 || f->posY < 0 || (f->posX+TILE_SIZE) >= SCREEN_WIDTH || (f->posY+TILE_SIZE) >= SCREEN_HEIGHT)
        return 1;
    else if (mapTile == BLOCK || mapTile == CRATE || mapTile == FRUIT || map[f->posY][f->posX].playerOn == 1 || mapTile == END)
        return 1;
    return 0;
}
char checkThrowCollision (Fruit* f)
{
    tile_t mapTile = checkType(map, f->posX, f->posY);
    if (f->posX < 0 || f->posY < 0 || (f->posX+TILE_SIZE) >= SCREEN_WIDTH || (f->posY+TILE_SIZE) >= SCREEN_HEIGHT)
        return 1;
    else if (mapTile == END)
        return 1;
    return 0;
}

Fruit* checkForFruitAtPosition(int x, int y) {
    int i;
    for (i = 0; i < NUM_PLAYERS*FRUITS_PER_PLAYER; i++)
        if (fruitCtrl->fruits[i].posX == x && fruitCtrl->fruits[i].posY == y)
            return &(fruitCtrl->fruits[i]);
        return NULL;
    }
