
#include <stdlib.h>
#include <stdio.h>

#include "Map.h"

char** initMap ()
{
	char** d = (char**)malloc(NTILEY*sizeof(char*));
	int i;
	for (i = 0; i < NTILEY; i++) {
		d[i] = (char*)malloc(NTILEX*sizeof(char));
	}

	int y;
	for (y = 0; y < NTILEY; y++) {
		int x;
		for (x = 0; x < NTILEX; x++) {
			d[y][x] = maplayout[y*NTILEX+x] - '0';
		}
	}

	return d;
}

void destroyMap (char** d)
{
	int i;
	for (i = 0; i < NTILEY; i++) {
		free (d[i]);
	}
	free (d);
}

char dropFruit (char** d, int x, int y)
{
	x = x_to_tx (x);
	y = y_to_ty (y);
	if (d[y][x] == FRUIT) {
		return 0;
	} else {
		d[y][x] = FRUIT;
		return 1;
	}
}

tile_t checkType (char** d, int x, int y)
{
	if (y < SCREEN_HEIGHT && x < SCREEN_WIDTH) {
		x = x_to_tx (x);
		y = y_to_ty (y);
		return d[y][x];
	} else {
		return END;
	}
}

char x_to_tx (int x)
{
	return x / TILE_SIZE;
}

char y_to_ty (int y)
{
	return y / TILE_SIZE;
}

void renderMap (char** d)
{

}

void debugPrint (char** d)
{
	int y;
	for (y = 0; y < NTILEY; y++) {
		int x;
		for (x = 0; x < NTILEX; x++) {
			printf("%i", d[y][x]);
		}
		printf("\n");
	}
}
