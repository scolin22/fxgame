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
			fruitCtrl->fruits[j].posX = 0;
			fruitCtrl->fruits[j].posY = 0;
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
	} else {
		return GRASS;
	}
}
void explodeFruit(FruitCtrl *fruitCtrl, Fruit fruit) {

	changeTile(fruitCtrl->map, fruit.posX, fruit.posY, EXPLOSION);

	tile_t currentTile;
	int x, y;
	//Check left
	int i;
	y = fruit.posY;
	for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
		x = fruit.posX - i;
		currentTile = checkExplosion(fruitCtrl, x, y);
		if (!explodeTile(fruitCtrl->map, x, y, currentTile))
			break;
	}
	for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
		x = fruit.posX + i;
		currentTile = checkExplosion(fruitCtrl, x, y);
		if (!explodeTile(fruitCtrl->map, x, y, currentTile))
			break;
	}
	x = fruit.posX;
	for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
		y = fruit.posY - i;
		currentTile = checkExplosion(fruitCtrl, x, y);
		if (!explodeTile(fruitCtrl->map, x, y, currentTile))
			break;
	}
	for (i = TILE_SIZE; i <= fruit.radius*TILE_SIZE; i += TILE_SIZE) {
		y = fruit.posY + i;
		currentTile = checkExplosion(fruitCtrl, x, y);
		if (!explodeTile(fruitCtrl->map, x, y, currentTile))
			break;
	}
}

char explodeTile(mapTile** map, int x, int y, tile_t tile) {
	if (tile == BLOCK)
		return 0;
	else if (tile == CRATE) {
		changeTile(map, x, y, EXPLOSION);
		return 0;
	}
	else if (tile == FRUIT) {
		changeTile(map, x, y, EXPLOSION);
		return 0;
	}
	else {
		changeTile(map, x, y, EXPLOSION);
		return 1;
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

char dropFruit(FruitCtrl *fruitCtrl, int owner, int x, int y){
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
			fruitCtrl->numFruits[owner] += 1;
			changeTile(fruitCtrl->map, x, y, FRUIT);
			break;
		}
	}
	return 1;
}
