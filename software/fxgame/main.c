
#include <stdio.h>
#include <unistd.h>
#include <system.h>
#include <io.h>
#include <time.h>

#include "Map.h"

#define switches (volatile char *) 0x0004430
#define leds (char *) 0x0004420

#define FPS 2.0
#define TICKS_PER_FRAME (1000.0 / FPS)

int main()
{
	char** map = initMap();
	//initialize other stuff such as vga, sd card, etc.
	alt_up_pixel_buffer_dma_dev *pixel_buffer = init_pixel_stuff("/dev/pixel_buffer_dma");
	renderMap(map, pixel_buffer);
	refresh(pixel_buffer);
	//clock_t start, stop;
	while (1) {
		//start = clock();



		//handle inputs (adjust player velocity, drop a fruit)
		//move the players (change x,y coordinates, check collisions)
		//render everything onto the screen



		//CAP FRAMERATE
		/*stop = clock();
		printf("%f\n", (((float)stop - (float)start) / CLOCKS_PER_SEC));
		if ((((float)stop - (float)start) / CLOCKS_PER_SEC) > TICKS_PER_FRAME) {
			usleep((TICKS_PER_FRAME - (((float)stop - (float)start) / CLOCKS_PER_SEC)));
		}*/

		IOWR(leds, 0, IORD(switches, 0));
	}
	destroyMap(map);
	return 0;
}
