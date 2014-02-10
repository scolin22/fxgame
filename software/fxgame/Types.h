#ifndef TYPES_H_
#define TYPES_H_

#define NUM_PLAYERS 2

typedef enum direction {
	none,
	left,
	right,
	up,
	down
} direction;

typedef enum fruitType {
    noType,
    normal,
    banana,
    watermelon,
    grapefruit,
    cherry
} fruitType;

#endif /* TYPES_H_ */
