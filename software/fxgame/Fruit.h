#ifndef FRUIT_H
#define FRUIT_H

typedef struct Fruit {
	int x; // x coordinate of the tile that the bomb is placed on
	int y; // y coordinate of the tile that the bomb is placed on
	int timer; // time in (milli)seconds when the bomb is placed
	int exploded = 0; //1 or 0 indicates whether or not the bomb has exploded, this might be unnecessary
	int power; // indicates how far the explosion will go
} Fruit;

#endif //FRUIT_H
