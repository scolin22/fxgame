// ONLY READFILEBYTES WORKS RIGHT NOW
// CHECK readbmp.c TO SEE HOW SDCARD API WORKS
#ifndef BMP_H_
#define BMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "altera_up_sd_card_avalon_interface.h"

#define DEBUG 0

int initSD(int *connected);

int readDir(char *dir, char *file);

int readDirMore(char *file);

int writeFile(char *file, bool create, char *content);

int readFile(char *file, bool create, char *result);

// close file after done reading to reset next byte pointer
int readFileBytes(char *file, int bytes, char *result, FILE *fp);

int closeFile(char *file, FILE* fp);

#endif
