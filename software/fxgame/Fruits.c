#include <stdlib.h>
#include <stdio.h>

#include "Fruits.h"

void initFruits(FruitCtrl *fruitCtrl, char** d) {
    int i = 0;fruitCtrl->map = d;
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
                }
                else if (fruitCtrl->fruits[i].status == exploding) {
                    fruitCtrl->fruits[i].status = hidden;
                    if (fruitCtrl->numFruits[fruitCtrl->fruits[i].owner] >= 1)
                        fruitCtrl->numFruits[fruitCtrl->fruits[i].owner] -= 1;
                    cleanExplosion(fruitCtrl, fruitCtrl->fruits[i]);
                    for (i = 0; i < NUM_PLAYERS*FRUITS_PER_PLAYER; i++)
                        if (fruitCtrl->fruits[i].timeLeft > 0 || fruitCtrl->fruits[i].status == exploding)
                            explodeFruit(fruitCtrl, fruitCtrl->fruits[i]);
                }
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
            if(fruitCtrl->fruits[i].x_pos == x && fruitCtrl->fruits[i].y_pos == y) {
                if (fruitCtrl->fruits[i].status == active) {
                    fruitCtrl->fruits[i].status = exploding;
                    fruitCtrl->fruits[i].timeLeft = EXPLOSION_TIMEOUT;
                    if (fruitCtrl->numFruits[fruitCtrl->fruits[i].owner] >= 1)
                        fruitCtrl->numFruits[fruitCtrl->fruits[i].owner] -= 1;
                    explodeFruit(fruitCtrl, fruitCtrl->fruits[i]);
                }
                break;
            }
        }
        return 2;
    } else {
        return 0;
    }
}
void explodeFruit(FruitCtrl *fruitCtrl, Fruit fruit) {
    changeTile(fruitCtrl->map, fruit.x_pos, fruit.y_pos, EXPLOSION);

    //Check left
    int i;
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.x_pos - i, fruit.y_pos) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.x_pos - i, fruit.y_pos)== 2) {
            changeTile(fruitCtrl->map, fruit.x_pos - i, fruit.y_pos, EXPLOSION);
            break;
        }
        else
            changeTile(fruitCtrl->map, fruit.x_pos - i, fruit.y_pos, EXPLOSION);
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.x_pos + i, fruit.y_pos) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.x_pos + i, fruit.y_pos)== 2) {
            changeTile(fruitCtrl->map, fruit.x_pos + i, fruit.y_pos, EXPLOSION);
            break;
        }
        else
            changeTile(fruitCtrl->map, fruit.x_pos + i, fruit.y_pos, EXPLOSION);
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.x_pos, fruit.y_pos - i) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.x_pos, fruit.y_pos - i)== 2) {
            changeTile(fruitCtrl->map, fruit.x_pos, fruit.y_pos - i, EXPLOSION);
            break;
        }
        else
            changeTile(fruitCtrl->map, fruit.x_pos, fruit.y_pos - i, EXPLOSION);
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.x_pos, fruit.y_pos + i) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.x_pos, fruit.y_pos + i)== 2) {
            changeTile(fruitCtrl->map, fruit.x_pos, fruit.y_pos + i, EXPLOSION);
            break;
        }
        else
            changeTile(fruitCtrl->map, fruit.x_pos, fruit.y_pos + i, EXPLOSION);
    }
}

void cleanExplosion(FruitCtrl *fruitCtrl, Fruit fruit) {
    checkExplosion(fruitCtrl, fruit.x_pos, fruit.y_pos);
    if (checkType(fruitCtrl->map, fruit.x_pos, fruit.y_pos) == EXPLOSION)
        changeTile(fruitCtrl->map, fruit.x_pos, fruit.y_pos, GRASS);

    //Check left
    int i;
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.x_pos - i, fruit.y_pos) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.x_pos - i, fruit.y_pos)== 2) {
            if (checkType(fruitCtrl->map, fruit.x_pos - i, fruit.y_pos) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.x_pos - i, fruit.y_pos, GRASS);
            break;
        }
        else {
            if (checkType(fruitCtrl->map, fruit.x_pos - i, fruit.y_pos) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.x_pos - i, fruit.y_pos, GRASS);
        }
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.x_pos + i, fruit.y_pos) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.x_pos + i, fruit.y_pos)== 2) {
            if (checkType(fruitCtrl->map, fruit.x_pos + i, fruit.y_pos) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.x_pos + i, fruit.y_pos, GRASS);
            break;
        }
        else {
            if (checkType(fruitCtrl->map, fruit.x_pos + i, fruit.y_pos) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.x_pos + i, fruit.y_pos, GRASS);
        }
    }
    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.x_pos, fruit.y_pos - i) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.x_pos, fruit.y_pos - i)== 2) {
            if (checkType(fruitCtrl->map, fruit.x_pos, fruit.y_pos - i) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.x_pos, fruit.y_pos - i, GRASS);
            break;
        }
        else {
            if (checkType(fruitCtrl->map, fruit.x_pos, fruit.y_pos - i) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.x_pos, fruit.y_pos - i, GRASS);
        }
    }

    for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
        if (checkExplosion(fruitCtrl, fruit.x_pos, fruit.y_pos + i) == 1)
            break;
        else if (checkExplosion(fruitCtrl, fruit.x_pos, fruit.y_pos + i)== 2) {
            if (checkType(fruitCtrl->map, fruit.x_pos, fruit.y_pos + i) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.x_pos, fruit.y_pos + i, GRASS);
            break;
        }
        else {
            if (checkType(fruitCtrl->map, fruit.x_pos, fruit.y_pos + i) == EXPLOSION)
                changeTile(fruitCtrl->map, fruit.x_pos, fruit.y_pos + i, GRASS);
        }
    }

}

char dropFruit(FruitCtrl *fruitCtrl, int owner, int x, int y){
    if (checkType (fruitCtrl->map, x, y) == FRUIT || fruitCtrl->numFruits[owner] >= fruitCtrl->maxFruits[owner]) {
        return 0;
    }
    int i;
    for (i = fruitCtrl->startIndex[owner]; i < (owner < NUM_PLAYERS - 1 ? fruitCtrl->startIndex[owner + 1] : NUM_PLAYERS*FRUITS_PER_PLAYER); i++) {
        if (fruitCtrl->fruits[i].status == hidden) {
            fruitCtrl->fruits[i].status = active;
            fruitCtrl->fruits[i].owner = owner;
            fruitCtrl->fruits[i].x_pos = x;
            fruitCtrl->fruits[i].y_pos = y;
            fruitCtrl->fruits[i].timeLeft = FRUIT_TIMEOUT;
            fruitCtrl->numFruits[owner] += 1;
            changeTile(fruitCtrl->map, x, y, FRUIT);
            break;
        }
    }
    return 1;
}
