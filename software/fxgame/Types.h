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
    normal,
    banana,
    cherry,
    orange,
    watermelon
} fruitType;

#endif /* TYPES_H_ */
