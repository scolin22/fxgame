#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <system.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "queue_type.h"
#include "animations.h"

#define DEBUG 0

int main(void) {
    printf("Hello Colin, from Nios II!\n");

    int connected = 0;
	alt_up_sd_card_dev *device_reference = NULL;
	device_reference = alt_up_sd_card_open_dev("/dev/sd_card");

	printf("Initialized\n");

	if (device_reference != NULL) {
		printf("Device reference found\n");
		while(1) {
			if ((connected == 0) && (alt_up_sd_card_is_Present())) {
				printf("Card connected.\n");

				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");
				} else {
					printf("Unknown file system.\n");
				}

				connected = 1;

				int fileNameLength = 100;
				char root[] = "/";
				char *file;
				file = malloc(sizeof(char) * fileNameLength);

				int result;

				result = alt_up_sd_card_find_first(*root, *file);
				if (DEBUG) printf("Operation result: %d\n", result);
				printf("File: %s\n", *file);

				while (result == 0) {
					result = alt_up_sd_card_find_next(*file);
					if (result != 0) break;
					if (DEBUG) printf("Operation result: %d\n", result);
					printf("File: %s\n", *file);
				}

			} else if ((connected == 1) && (alt_up_sd_card_is_Present() == false)) {
				printf("Card disconnected.\n");
				connected = 0;
			}
		}
	} else {
		printf("Device reference not found\n");
	}

	return 0;

    alt_up_char_buffer_dev *char_buffer = init_char_stuff("/dev/char_drawer");

    //Write some text
    alt_up_char_buffer_string(char_buffer, "WELCOME TO FRUIT EXPLOSION", 27, 29);
    alt_up_char_buffer_string(char_buffer, "THE FRUIT EXPLODING GAME", 28, 31);
    alt_up_char_buffer_string(char_buffer, "GAME COMING SOON", 32, 36);
    alt_up_char_buffer_string(char_buffer, "BY BYRON", 36, 38);

    alt_up_pixel_buffer_dma_dev *pixel_buffer = init_pixel_stuff("/dev/pixel_buffer_dma");

    //Do your thing
    Queue * x0_queue = queue_new(20);
    Queue * y0_queue = queue_new(20);
    Queue * x1_queue = queue_new(20);
    Queue * y1_queue = queue_new(20);
    // while(1) {
    //     // spinner(pixel_buffer, x0_queue, x1_queue, y0_queue, y1_queue);
    //     pulsing_box(pixel_buffer);
    // }

    infinite_spinner(pixel_buffer, x0_queue, x1_queue, y0_queue, y1_queue);
    return 0;
}
